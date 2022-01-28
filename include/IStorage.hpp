/***
* File: IStorage.hpp
* Date: 13/09/2021

***/


#ifndef ILRD_HRD18_I_STORAGE
#define ILRD_HRD18_I_STORAGE

#include <stdexcept>
#include <memory>

#include "DriverData.hpp"

namespace HRD18
{

class IStorage
{
public:
    explicit IStorage() = default;
    IStorage(const IStorage&) = delete;
    IStorage& operator=(const IStorage&) = delete;
    virtual ~IStorage() noexcept = default;

    virtual void Read(std::shared_ptr<DriverData> data) = 0;
    virtual void Write(std::shared_ptr<DriverData> data) = 0;
    
};

class StorageError : public std::runtime_error
{
    explicit StorageError(const std::string& msg);
    ~StorageError() = default;
};

}//namespace HRD18

#endif
