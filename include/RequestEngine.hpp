/*****************************************************
* File: 	   RequestEngine.hpp                     *
* Date: 	   04/10/2021							 *
* Description: RequestEngine API - Command DP        * 
******************************************************/
#ifndef ILRD_HRD18_REQUEST_ENGINE_HPP
#define ILRD_HRD18_REQUEST_ENGINE_HPP

#include <vector>

#include "IGateway.hpp"
#include "Factory.hpp"
#include "ThreadPool.hpp"
#include "IRETask.hpp"

namespace HRD18
{

// T - The data object
// Key - The key for the object creation in factory
// Args - the parameters for the object construction  in factory

template <typename T, typename Key, typename Args>
class RequestEngine
{
public:
    explicit RequestEngine() = default;
    ~RequestEngine() = default; 
    RequestEngine& operator=(const RequestEngine& rhs) = delete;
    RequestEngine(const RequestEngine& other) = delete;
    void Configure(std::vector<IGateway<T> *> gateways, std::vector<IRETask *> tasks/*...*/);
    // may throw NBDDriverError, StorageError
    void Run();
private:
    std::vector<IGateway<T> *> m_gateways;
    Factory<IRETask, Key, Args> m_factory;
    ThreadPool m_threadpool;
}; //class RequestEngine

} //namespace HRD18

#endif //ILRD_HRD18_REQUEST_ENGINE_HPP