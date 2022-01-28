/***
* File: RAMStorage.hpp
* Date: 14/09/2021

***/


#ifndef ILRD_HRD18_RAM_STORAGE
#define ILRD_HRD18_RAM_STORAGE

#include <cstddef> //size_t
#include <stdexcept>

#include "IStorage.hpp"

namespace HRD18
{


class RAMStorage : public IStorage
{
public:
    explicit RAMStorage(size_t size);
    ~RAMStorage() = default;

    void Read(std::shared_ptr<DriverData> data) override;
    void Write(std::shared_ptr<DriverData> data) override;

private:
    size_t m_size;
    std::vector<char> m_drive;

};

class RAMStorageError : public StorageError
{
    explicit RAMStorageError(const std::string& msg);
    ~RAMStorageError() = default;
};

}//namespace HRD18

#endif
