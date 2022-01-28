/*****************************************************
* File: 	   WPriorityQ.hpp                     	 	 *
* Date: 	   30/09/2021							 * 
* Version: 	   1.0  								 *
* Description: Wait Priority Queue API                           * 
******************************************************/

#ifndef ILRD_HRD18_WPRIORITYQ
#define ILRD_HRD18_WPRIORITYQ

#include <queue> //std::priority_queue
#include <mutex> //std::mutex
#include <condition_variable> //std::condition_variable

namespace HRD18
{
template<typename T,
         typename Container = std::vector<T>,
         typename Compare = std::less<typename Container::value_type>>
         // instead of Container::value_type can use T but 
         // this is how this declared in cpprefrence

class WPriorityQ
{
public:
    explicit WPriorityQ(const Compare& compare = Compare());
    ~WPriorityQ() = default;

    bool Pop(T& retVal, std::chrono::seconds timeOut); 
    void Push(const T& value);
    size_t Size() const;
    bool IsEmpty() const;
private:
    WPriorityQ(const WPriorityQ& other) = delete;
    WPriorityQ& operator=(const WPriorityQ& rhs) = delete;

    std::priority_queue<T, Container, Compare> m_queue;
    mutable std::mutex m_queueMutex;
    std::condition_variable m_queueCon;
    std::mutex m_queueConMutex;
};

template<typename T, typename Container, typename Compare>
WPriorityQ<T, Container, Compare>::WPriorityQ(const Compare& compare) : m_queue(compare)
{}
            

template<typename T, typename Container, typename Compare>
bool WPriorityQ<T, Container, Compare>::Pop(T& retVal, std::chrono::seconds timeOut)
{
    std::unique_lock<std::mutex> q_con_lock(m_queueConMutex);
    bool res = m_queueCon.wait_for(q_con_lock, timeOut, [&]{return !IsEmpty();});
    if (!res)
    {
        return false;
    }
    std::lock_guard<std::mutex> q_lock(m_queueMutex);
    retVal = m_queue.top();
    m_queue.pop();
    return true;
}

template<typename T, typename Container, typename Compare>
void WPriorityQ<T, Container, Compare>::Push(const T& value)
{
    std::lock_guard<std::mutex> lock(m_queueMutex);
    m_queue.push(value);
    m_queueCon.notify_one();
}

template<typename T, typename Container, typename Compare>
size_t WPriorityQ<T, Container, Compare>::Size() const
{
    std::lock_guard<std::mutex> lock(m_queueMutex);
    return (m_queue.size());
}


template<typename T, typename Container, typename Compare>
bool WPriorityQ<T, Container, Compare>::IsEmpty() const
{
    std::lock_guard<std::mutex> lock(m_queueMutex);
    return (m_queue.empty());
}

} //namespace HRD18

#endif // ILRD_HRD18_FACTORY