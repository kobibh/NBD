#include <cstring>
#include "IStorage.hpp"
#include "RAMStorage.hpp"
#include <iostream>


namespace HRD18
{
    RAMStorage::RAMStorage(size_t size) : m_size(size) ,m_drive(size)
    {}

    void RAMStorage::Read(std::shared_ptr<DriverData> data)
    {
        auto it_src = m_drive.begin() + data->m_offset;
        auto it_dest = data.get()->m_data.begin();
        for(;it_src < m_drive.begin() + data->m_len + data->m_offset; ++it_src, ++it_dest)
        {
            *it_dest = *it_src;
        }    
    }

    void RAMStorage::Write(std::shared_ptr<DriverData> data)
    {
        std::vector<char>::iterator it_src = data.get()->m_data.begin() ;
        std::vector<char>::iterator it_dest =  m_drive.begin() + data.get()->m_offset;
        for(;it_src < data->m_data.end(); ++it_src, ++it_dest)
        {
            *it_dest = *it_src;
        }   
    }
}
  