/***
* File: DriverData.hpp
* Date: 14/09/2021

***/


#ifndef ILRD_HRD18_DRIVER_DATA
#define ILRD_HRD18_DRIVER_DATA

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace HRD18
{


struct DriverData
{

    enum RequestType
    {
        READ, WRITE, DISC, FLUSH, TRIM, 
    };

    // len is the number of bytes to read or write
    explicit DriverData(const char* handle, RequestType type, size_t offset, size_t len);
    DriverData(const DriverData&) = delete;
    DriverData& operator=(const DriverData&) = delete;
    ~DriverData() = default;

    static const int HANDLE_SIZE = 8;
    char m_handle[HANDLE_SIZE];
    RequestType m_type;
    size_t m_offset;
    size_t m_len;
    int m_status;
    std::vector<char> m_data;

};

}//namespace HRD18

#endif