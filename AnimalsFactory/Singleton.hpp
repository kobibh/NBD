/***
* File: Singleton.hpp
* Date: 23/09/2021
* Singleton requirements:
* 1. T ctor's should be private
* 2. T should implement default c'tor
* 3. T ctor's should be friend with Singleton
***/


#ifndef ILRD_HRD18_SINGLETON
#define ILRD_HRD18_SINGLETON

#include <mutex> // std::mutex
#include <atomic>
#include <memory>

namespace HRD18
{

template <typename T>
class Singleton
{
public:
    explicit Singleton() = delete;
    ~Singleton() = delete;
    Singleton(const Singleton& other) = delete;
    Singleton& operator=(const Singleton& rhs) = delete;
    static T* GetInstance();
private:
    static std::atomic<T*> s_instance;
    static std::mutex s_mutex;
};

template<typename T>
std::atomic<T*>  Singleton<T>::s_instance;

template<typename T>
std::mutex Singleton<T>::s_mutex;

template <typename T>
T* Singleton<T>::GetInstance()
{
    T* tmp = s_instance.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);
    if (tmp == nullptr) {
        std::lock_guard<std::mutex> lock(s_mutex);
        tmp = s_instance.load(std::memory_order_relaxed);
        if (tmp == nullptr)
        {
            tmp = new T();
            static std::unique_ptr<T> deleter(tmp);
            std::atomic_thread_fence(std::memory_order_release);
            s_instance.store(tmp, std::memory_order_relaxed);
        }
    }
    return tmp;
}

}


#endif // ILRD_HRD18_SINGLETON
