#include <memory>
#include <iostream>

#include "ThreadPool.hpp"
#include "ITask.hpp"
#include "Singleton.hpp"
#include "Logger.hpp"
#include <algorithm>

namespace HRD18
{

class BadAppleException : public std::runtime_error
{
public:
    explicit BadAppleException(const char* msg);

};

class BadApple : public ITask
{
public:
    BadApple(ITask::Priority priority);
    ~BadApple();
    virtual void Execute();
};



ThreadPool::ThreadPool(size_t num_of_threads) : m_threadNum(num_of_threads),
                                                m_logger(Singleton<Logger>::GetInstance()),
                                                m_pool(num_of_threads + (num_of_threads == 0))
                                                
{
    // for(std::thread& thread : m_pool)
    // {
    //     thread = std::thread(&ThreadPool::ThreadFunc, this);
    // }

    std::for_each(m_pool.begin(), m_pool.end(), 
    [&](std::thread& thread)
    {
        thread = std::thread(&ThreadPool::ThreadFunc, this);
    }
    );
    m_logger->SetLogLevel(Logger::LOG_LEVEL::DEBUG);                           
}       

ThreadPool::~ThreadPool()
{
    Stop();
}

void ThreadPool::AddTask(TaskPtr task)
{
    m_wpq.Push(task);
    m_logger->log(Logger::LOG_LEVEL::DEBUG, "Task pushed", __FILE__, __LINE__, true);
}


void ThreadPool::Stop()
{
    TaskPtr bad_apple = std::make_shared<BadApple>(ITask::Priority::STOP);
    
    for(std::thread& thread : m_pool)
    {
        AddTask(bad_apple);m_logger->log(Logger::LOG_LEVEL::DEBUG, "Bad Apple pushed", __FILE__, __LINE__, true);
        // thread.join(); - mistake!!! - before i implemented the join loop i did this.
        // the mistake is that i cant i waited the a thread but i dont know who caught the bad apple
        // so the pop is stuck...
    }
    for(std::thread& thread : m_pool)
    {
        thread.join();   
    }
    // for (size_t i = 0; i < m_threadNum; ++i)
    // {
    //     AddTask(bad_apple);m_logger->log(Logger::LOG_LEVEL::DEBUG, "Bad Apple pushed", __FILE__, __LINE__, true);
    // }
    // for (size_t i = 0; i < m_threadNum; ++i)
    // {
    //     m_pool.at(i).join();
    // }
}

void ThreadPool::ForceStop(std::chrono::seconds time_out)
{}


void ThreadPool::SetSize(size_t new_size)
{
    if (new_size == m_threadNum)
    {
        return;
    }
    else
    {
        if (new_size < m_threadNum)
        {
            size_t bad_apples = m_threadNum - new_size;
            m_threadNum = new_size;
            AddBadApples(bad_apples);
        }
        else
        {
            size_t new_threads = new_size - m_pool.size();
            auto it_from = m_pool.begin() + m_threadNum;
            for (size_t i = 0; i < new_threads; i++, ++it_from)
            {
                m_pool.insert(it_from, std::thread(&ThreadPool::ThreadFunc, this));
            }   
            m_threadNum = new_size;
        }   
    }
#ifdef _DEBUG
    m_logger->log(Logger::LOG_LEVEL::DEBUG, "GetContainerSize()" + std::to_string(GetContainerSize()) , __FILE__, __LINE__, true);
#endif
}


void ThreadPool::AddBadApples(size_t bad_apples)
{
    TaskPtr bad_apple = std::make_shared<BadApple>(ITask::Priority::STOP);
    
    for(size_t i = 0; i < bad_apples; ++i)
    {
        AddTask(bad_apple);m_logger->log(Logger::LOG_LEVEL::DEBUG, "Bad apple pushed - setsize", __FILE__, __LINE__, true);
    }
}

void ThreadPool::Suspend()
{

}

void ThreadPool::Resume()
{

}


bool ThreadPool::Compare::operator()(TaskPtr& lhs, TaskPtr& rhs)
{
    return *lhs < *rhs;
}



void ThreadPool::ThreadFunc()
{
    TaskPtr ret;
    
    std::chrono::seconds timeout(999999999);
    while(m_wpq.Pop(ret, timeout))
    {
        try
        {   
           (*ret).Execute();
        }
        catch(const BadAppleException& e)
        {
            break;
        }
    }

}

BadApple::BadApple(ITask::Priority priority) : ITask(priority)
{}

BadApple::~BadApple()
{}

void BadApple::Execute()
{    
    throw BadAppleException("BadApple : stop");
}



BadAppleException::BadAppleException(const char* msg) : std::runtime_error(msg)
{}

#ifdef _DEBUG
size_t ThreadPool::GetContainerSize()
{
    return m_pool.size();
}
#endif

}//namespace HRD18