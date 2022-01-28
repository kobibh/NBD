/*****************************************************
* File: 	   IGateway.hpp                           *
* Date: 	   04/10/2021							 *
* Description: Gateway interface API - Command DP       * 
******************************************************/

#ifndef ILRD_HRD18_IGATEWAY_HPP
#define ILRD_HRD18_IGATEWAY_HPP


#include <memory> // std::shared_ptr

namespace HRD18
{

template <typename T>
class IGateway
{

public:
    explicit IGateway(int fd);
    ~IGateway() = default;
    IGateway& operator=(const IGateway&) = delete;
    IGateway(const IGateway&) = delete;
    virtual std::shared_ptr<T> Read() = 0;

private:
    int fd;
  
};

}

#endif // ILRD_HRD18_IGateway_HPP