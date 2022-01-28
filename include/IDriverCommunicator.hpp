/***
* File: IDriverCommunicator.hpp
* Date: 13/09/2021

***/


#ifndef ILRD_HRD18_I_DRIVER_COMMUNICATOR
#define ILRD_HRD18_I_DRIVER_COMMUNICATOR

#include <string>
#include <stdexcept>
#include <memory> /* shared_ptr */

#include "DriverData.hpp"
#include "IStorage.hpp"

namespace HRD18
{

class IDriverCommunicator
{
public:
    explicit IDriverCommunicator() = default;
    virtual ~IDriverCommunicator() noexcept = default;
    IDriverCommunicator(const IDriverCommunicator&) = delete;
    IDriverCommunicator& operator=(const IDriverCommunicator&) = delete;

    virtual std::shared_ptr<DriverData> RetrieveRequest() = 0;
    virtual void SendReply(std::shared_ptr<DriverData>) = 0;
    virtual void Disconnect() = 0;
    virtual int GetRequestFD() = 0;
};

class DriverError : public std::runtime_error
{
public:
    explicit DriverError(const std::string& msg);
    ~DriverError() = default;
};




}//namespace HRD18

#endif
