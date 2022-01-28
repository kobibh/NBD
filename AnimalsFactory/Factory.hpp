/*****************************************************
* File: 	   Factory.cpp                     	 	 *
* Date: 	   29/09/2021							 * 
* Version: 	   1.0  								 *
* Description: Factory API                           * 
******************************************************/

#ifndef ILRD_HRD18_FACTORY
#define ILRD_HRD18_FACTORY

#include <unordered_map>
#include <memory>
#include <functional>
#include "Singleton.hpp"
namespace HRD18
{

template <typename Base, typename Key, typename Args>
class Factory
{
public:
    using CreateFunction = std::function<std::shared_ptr<Base>(Args args)>;

    ~Factory() = default;

    //If key already exists - updates the function
    //May throw bad_alloc exception
    void Add(const Key& key, CreateFunction CreateFunc);

    //May throw bad_alloc exception
    std::shared_ptr<Base> Create(const Key& key, Args args);  

private:
    //Factory is a singleton 
    explicit Factory() = default;
    
    Factory(const Factory& other) = delete;
    Factory& operator=(const Factory& rhs) = delete;

    friend class Singleton<Factory>;

    std::unordered_map<Key, CreateFunction> m_createTable;
};

template <typename Base, typename Key, typename Args>
void Factory<Base, Key, Args>::Add(const Key& key, CreateFunction CreateFunc)
{
    std::pair<Key,CreateFunction> new_pair(key, CreateFunc);
    m_createTable.insert(new_pair);
}


template <typename Base, typename Key, typename Args>
std::shared_ptr<Base> Factory<Base, Key, Args>::Create(const Key& key, Args args)
{
    return (m_createTable[key](args));
}

} //namespace HRD18

#endif // ILRD_HRD18_FACTORY


