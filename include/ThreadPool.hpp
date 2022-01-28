/*****************************************************
* File: 	   threadpool.hpp                     	 *
* Date: 	   04/10/2021							 *
* Description: Thread Pool API                       * 
******************************************************/

#ifndef ILRD_HRD18_THREADPOOL
#define ILRD_HRD18_THREADPOOL

#include <memory> // shared_ptr
#include <vector> // vector
#include <thread> // std::thread
#include <chrono> // std::chrono::seconds time_out

#include "WPriorityQ.hpp"
#include "ITask.hpp"
#include "Singleton.hpp"
#include "Logger.hpp"

namespace HRD18
{

class ThreadPool
{
public:
    using TaskPtr = std::shared_ptr<ITask>;
    // default is hardware_concurrency, but if this cannot be calculated the default is 1
    // may throw 
    explicit ThreadPool(size_t num_of_threads = std::thread::hardware_concurrency());
    ~ThreadPool();
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool& rhs) = delete;
    
    // may throw a bad_alloc exception
    void AddTask(std::shared_ptr<ITask> task);

    // Finish executing tasks currently being handled
    void Stop();
    // Stop in the middle of executing
    void ForceStop(std::chrono::seconds time_out);

    // may throw bad_alloc
    void SetSize(size_t new_size);

    void Suspend();
    void Resume();

#ifdef _DEBUG
    size_t GetContainerSize();
#endif 
private:
    class Compare
    {
    public:
        bool operator()(std::shared_ptr<ITask>& lhs, std::shared_ptr<ITask>& rhs);
    };
    size_t m_threadNum;
    Logger *m_logger;
    std::vector<std::thread> m_pool;
    WPriorityQ<std::shared_ptr<ITask>, std::vector<std::shared_ptr<ITask>>, ThreadPool::Compare> m_wpq;
    void ThreadFunc();
    void ResizeContainer(size_t bad_apples);
    void AddBadApples(size_t bad_apples);
};



} // namespace HRD18

#endif // ILRD_HRD18_THREADPOOL