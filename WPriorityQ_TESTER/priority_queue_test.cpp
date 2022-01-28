
#include <thread>       //std::thread
#include <vector>       //std::vector
#include <iostream>     //std::cout
#include <chrono>       //std::chrono::second
#include <functional>   //std::ref, std::cref
#include <sstream>      //std::stringstream 
#include <string>       //std::string

#include "WPriorityQ.hpp"
#include "Singleton.hpp"
#include "Logger.hpp"


using namespace HRD18;

template<typename Compare>
void QueueWriter(WPriorityQ<int, 
        std::vector<int>, Compare>& queue, const std::vector<int>& data)
{
    std::stringstream stream; 
    
    Logger* logger = Singleton<Logger>::GetInstance();

    for(int num: data)
    {
        stream << std::this_thread::get_id() << " Add " << num << " " << std::endl;

        // std::cout << stream.str();
        logger->Log(Logger::INFO, stream.str(), __FILE__, __LINE__, true);

        stream.str(std::string());
        queue.Push(num);
    }

    // std::this_thread::sleep_for(std::chrono::seconds(20));

    // for(int num: data)
    // {
    //     stream << std::this_thread::get_id() << " Add " << num << " " << std::endl;

    //     // std::cout << stream.str();
    //     logger->Log(Logger::INFO, stream.str(), __FILE__, __LINE__, true);
    //     stream.str(std::string());

    //     queue.Push(num);

    //     // std::this_thread::sleep_for(std::chrono::seconds(5));
    // }
}

template<typename Compare>
void QueueReader(WPriorityQ<int, std::vector<int>, Compare>& queue)
{
    int val = 0;
    using Second = std::chrono::seconds;
    Second dur(5);

    std::stringstream stream;

    Logger* logger = Singleton<Logger>::GetInstance();

    for(int i = 0; i < 20; ++i)
    {
        if(queue.Pop(val, dur))
        {
            stream << std::this_thread::get_id() << " Remove " << val << " " << std::endl;
            // std::cout << stream.str();
            logger->Log(Logger::INFO, stream.str(), __FILE__, __LINE__, true);

            stream.str(std::string());
        }
        else
        {
            stream << "Queue is empty" << std::endl;
            // std::cout << stream.str();

            logger->Log(Logger::INFO, stream.str(), __FILE__, __LINE__, true);

            stream.str(std::string());
        }
    }
}



int main()
{
    const std::vector<int> data = {1,8,5,6,3,4,10,9,7,2,1};
    
    Logger* logger = Singleton<Logger>::GetInstance();
    logger->SetLogLevel(Logger::INFO);

    // auto cmp = [](int left, int right) { return (left) < (right); };
    std::less<int> cmp;
    WPriorityQ<int, std::vector<int>, decltype(cmp)> queue(cmp);

    //---Single Threaded---

    std::cout << "Single-Threaded" << std::endl;

    for(int n : data)
    {
        queue.Push(n);
    }
    using Second = std::chrono::seconds;

    Second dur(1);

    while(!queue.IsEmpty())
    {
        int val = 0;
        queue.Pop(val, dur);
        std::cout << val << std::endl;
    }

    //----Multithreaded----
    std::cout << "Multi-Threaded" << std::endl;

    std::thread writer1(QueueWriter<decltype(cmp)>, std::ref(queue), std::cref(data));
    std::thread writer2(QueueWriter<decltype(cmp)>, std::ref(queue), std::cref(data));
    // std::thread writer3(QueueWriter<decltype(cmp)>, std::ref(queue), std::cref(data));
    std::thread reader1(QueueReader<decltype(cmp)>, std::ref(queue));
    std::thread reader2(QueueReader<decltype(cmp)>, std::ref(queue));
    // std::thread reader3(QueueReader<decltype(cmp)>, std::ref(queue));

    reader1.join();
    reader2.join();
    // reader3.join();
    writer1.join();
    writer2.join();
    // writer3.join();

    return 0;
}


