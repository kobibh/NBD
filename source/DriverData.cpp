#include <cstring>
#include "DriverData.hpp"

namespace HRD18
{

DriverData::DriverData(const char* handle, RequestType type, size_t offset, size_t len) :
                        m_handle(), m_type(type), m_offset(offset), m_len(len), m_data(len)
{
    std::memcpy(m_handle, handle, HANDLE_SIZE);
}

}

