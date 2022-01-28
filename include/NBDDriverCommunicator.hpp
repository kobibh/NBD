/***
* File: NBDDriverCommunicator.hpp
* Date: 13/09/2021
chane 0.3 : added thread member
***/


#ifndef ILRD_HRD18_NBD_DRIVER_COMMUNICATOR
#define ILRD_HRD18_NBD_DRIVER_COMMUNICATOR

#include <cstddef> //size_t
#include <stdexcept>
#include <thread>

#include "IDriverCommunicator.hpp"
#include "Singleton.hpp"
#include "Logger.hpp"

namespace HRD18
{

class NBDDriverCommunicator : public IDriverCommunicator
{
public:
    explicit NBDDriverCommunicator(const std::string& devPath, size_t storageSize);//storageSize is in bytes
    explicit NBDDriverCommunicator(const std::string& devPath, size_t blockCount, size_t blockSize);
    ~NBDDriverCommunicator() noexcept override; 

    std::shared_ptr<DriverData> RetrieveRequest() override;
    void SendReply(std::shared_ptr<DriverData> data) override;
    void Disconnect() override;
    int GetRequestFD() override;
    
private:
    const std::string& m_devPath;
    int m_dev_fd;
    int m_nbdSock;
    int m_appSock;
    Logger* m_logger;
    std::thread m_listener_thread;
    void NBDListener();
    void CleanUp();
};

class NBDDriverError : public DriverError
{
public:
    explicit NBDDriverError(const std::string& msg);
    ~NBDDriverError() = default;
};
}//namespace HRD18

#endif
