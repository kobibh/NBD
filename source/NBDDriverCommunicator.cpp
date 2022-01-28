/************************************
   File: NBDDriverCommunicator.cpp 
   date: 14/09/2021                
************************************/
#define _POSIX_C_SOURCE (200809L)

#include <assert.h>
#include <errno.h>
#include <err.h>
#include <fcntl.h>
#include <linux/nbd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <thread> // thread
#include <iostream> // cerr
#include <cerrno>


#include "IDriverCommunicator.hpp"
#include "NBDDriverCommunicator.hpp"
#include "DriverData.hpp"
#include "IStorage.hpp"
#include "RAMStorage.hpp"


#ifndef BUSE_DEBUG
  #define BUSE_DEBUG (0)
#endif


namespace HRD18
{
static int write_all(int fd, char* buf, size_t count);
static int read_all(int fd, char* buf, size_t count);
u_int64_t ntohll(u_int64_t a);
static void BlockAllSignals();
template <typename ...Args>
static int ioctl_wrapper(int fd, unsigned long request,Args ...args);


NBDDriverError::NBDDriverError(const std::string& msg) : DriverError(msg)
{}

//storageSize is in bytes
NBDDriverCommunicator::NBDDriverCommunicator(const std::string &devPath, size_t storageSize) 
    : m_devPath(devPath), m_logger(Singleton<Logger>::GetInstance())
{
    int sp[2];
            // creating socketpair 
    if(0 != socketpair(AF_UNIX, SOCK_STREAM, 0, sp))
    {
        m_logger->log(Logger::LOG_LEVEL::ERROR,
                    "socketpair(AF_UNIX, SOCK_STREAM, 0, sp) failed",
                    __FILE__,
                    __LINE__, true);
        throw NBDDriverError("socketpair(AF_UNIX, SOCK_STREAM, 0, sp) failed");
    }
    m_appSock = sp[0];
    m_nbdSock = sp[1];
    if(-1 == (m_dev_fd = open(m_devPath.c_str(), O_RDWR)))
    {
        m_logger->log(Logger::LOG_LEVEL::ERROR,
                    "open(m_devPath.c_str(), O_RDWR) failed",
                    __FILE__,
                    __LINE__, true);            
        throw NBDDriverError("open(m_devPath.c_str(), O_RDWR) failed");
    }

    if (-1 == ioctl_wrapper(m_dev_fd, NBD_SET_SIZE, storageSize))
    {
        
        m_logger->log(Logger::LOG_LEVEL::ERROR,
                        "ioctl(m_dev_fd, NBD_SET_SIZE, storageSize)",
                    __FILE__,
                    __LINE__,
                    true);
        throw NBDDriverError("ioctl(m_dev_fd, NBD_SET_SIZE, storageSize) failed");
    }
    
    if (-1 == ioctl_wrapper(m_dev_fd, NBD_CLEAR_SOCK))
    {
        
        m_logger->log(Logger::LOG_LEVEL::ERROR,
                        "ioctl(m_dev_fd, NBD_SET_SIZE, storageSize)",
                    __FILE__,
                    __LINE__,
                    true);
        throw NBDDriverError("ioctl(m_dev_fd, NBD_SET_SIZE, storageSize) failed");
    }
    m_listener_thread = std::thread(&NBDDriverCommunicator::NBDListener, this);
}

NBDDriverCommunicator::NBDDriverCommunicator(const std::string &devPath, size_t blockCount, size_t blockSize) : m_devPath(devPath)
{
    int sp[2];
    
    // creating socketpair 
    if(0 != socketpair(AF_UNIX, SOCK_STREAM, 0, sp))
    {
        m_logger->log(Logger::LOG_LEVEL::ERROR,
                    "socketpair(AF_UNIX, SOCK_STREAM, 0, sp) failed",
                    __FILE__,
                    __LINE__, true);
        throw NBDDriverError("socketpair(AF_UNIX, SOCK_STREAM, 0, sp) failed");
    }
    m_appSock = sp[0];
    m_nbdSock = sp[1];
    m_dev_fd = open(devPath.c_str(), O_RDWR);
    if (-1 == ioctl_wrapper(m_dev_fd, NBD_SET_BLKSIZE, blockSize))
    {
                
        m_logger->log(Logger::LOG_LEVEL::ERROR,
                        "ioctl(m_dev_fd, NBD_SET_BLKSIZE, blockSize)",
                    __FILE__,
                    __LINE__,
                    true);
        throw NBDDriverError("ioctl(m_dev_fd, NBD_SET_BLKSIZE, blockSize) failed");       
    }
    
    if (-1 == ioctl_wrapper(m_dev_fd, NBD_SET_SIZE_BLOCKS, blockCount))
    {
                
        m_logger->log(Logger::LOG_LEVEL::ERROR,
                        "ioctl(m_dev_fd, NBD_SET_BLKSIZE, blockCount)",
                    __FILE__,
                    __LINE__,
                    true);
        throw NBDDriverError("ioctl(m_dev_fd, NBD_SET_BLKSIZE, blockCount) failed");       
    }

    if (-1 == ioctl_wrapper(m_dev_fd, NBD_CLEAR_SOCK))
    {
        
        m_logger->log(Logger::LOG_LEVEL::ERROR,
                        "ioctl(m_dev_fd, NBD_SET_SIZE, storageSize)",
                    __FILE__,
                    __LINE__,
                    true);
        throw NBDDriverError("ioctl(m_dev_fd, NBD_SET_SIZE, storageSize) failed");
    }
    m_listener_thread = std::thread(&NBDDriverCommunicator::NBDListener, this);

}

NBDDriverCommunicator::~NBDDriverCommunicator()
{
    m_listener_thread.join();
    Disconnect();
}

// the app takes the requests from the socket 
std::shared_ptr<DriverData> NBDDriverCommunicator::RetrieveRequest()
{
    struct nbd_request request;
    ssize_t bytes_read;
    if (-1 == (bytes_read = read(m_appSock, &request, sizeof(struct nbd_request))))
    {
            m_logger->log(Logger::LOG_LEVEL::ERROR,
                        "RetrieveRequest : read",
                        __FILE__,
                        __LINE__,
                        true);
            throw NBDDriverError("RetrieveRequest : read_all failed");
    }
    if (bytes_read != sizeof(struct nbd_request))
    {
        throw NBDDriverError("RetrieveRequest :: fewer bytes were read");
    }

    if (request.magic != htonl(NBD_REQUEST_MAGIC))
    {
        throw NBDDriverError("RetrieveRequest :: magic numbers arent equal"); 
    }
    
    std::shared_ptr<DriverData> data = std::make_shared<DriverData>(request.handle,
                                    static_cast<DriverData::RequestType>(ntohl(request.type)),
                                    static_cast<size_t>(ntohll(request.from)),
                                    static_cast<size_t>(ntohl(request.len)));
    
    if (data->m_type == DriverData::RequestType::WRITE)
    {

        if (-1 == read_all(m_appSock, data->m_data.data(), data->m_len))
        {
            m_logger->log(Logger::LOG_LEVEL::ERROR,
                        "RetrieveRequest : read_all failed",
                        __FILE__,
                        __LINE__,
                        true);
            throw NBDDriverError("RetrieveRequest : read_all failed");
        }
    }

    return (data);
}


/* this the app job - send a reply */
void NBDDriverCommunicator::SendReply(std::shared_ptr<DriverData> request_data)
{
    struct nbd_reply reply;
    
    /* taking the the request data and put it in the reply */
    reply.error = 0;

    memcpy(reply.handle, request_data->m_handle, request_data->HANDLE_SIZE);
    reply.magic = htonl(NBD_REPLY_MAGIC);

    /* write the reply to the socket */
    if (-1 == write_all(m_appSock, reinterpret_cast<char *>(&reply) , sizeof(struct nbd_reply)))
    {
        m_logger->log(Logger::LOG_LEVEL::ERROR,
                        "SendReply : write_all failed",
                        __FILE__,
                        __LINE__,
                        true);
        throw NBDDriverError("SendReply : write_all failed");
    }
    
    /* also if it's a read request the data is written to the reply vector */
    if (request_data->m_type == DriverData::RequestType::READ)
    {
        if (-1 == write_all(m_appSock, request_data->m_data.data(), request_data->m_len))
        {
            m_logger->log(Logger::LOG_LEVEL::ERROR,
                        "SendReply : write_all failed",
                        __FILE__,
                        __LINE__,
                        true);
            throw NBDDriverError("SendReply : write_all failed");
        }
    }
}

void NBDDriverCommunicator::Disconnect()
{
    (void)signal;

    if (-1 == ioctl_wrapper(m_dev_fd, NBD_DISCONNECT))
    {
        m_logger->log(Logger::LOG_LEVEL::INFO,
                    "failed to request disconect on nbd device",
                    __FILE__,
                    __LINE__,
                    true);
        throw NBDDriverError("failed to request disconect on nbd device");
    } else
    {
        m_logger->log(Logger::LOG_LEVEL::INFO,
                        "sucessfuly requested disconnect on nbd device",
                        __FILE__,
                        __LINE__,
                        true);
    }
    
}
int NBDDriverCommunicator::GetRequestFD()
{
    return (m_appSock);
}

void NBDDriverCommunicator::NBDListener()
{
    int flags;
    BlockAllSignals();
    if (-1 == ioctl_wrapper(m_dev_fd, NBD_SET_SOCK, m_nbdSock))
    {
        m_logger->log(Logger::LOG_LEVEL::ERROR,
                        "ioctl(nbd, NBD_SET_SOCK, sk) failed",
                        __FILE__,
                        __LINE__,
                        true);
        throw NBDDriverError("ioctl(nbd, NBD_SET_SOCK, sk) failed");
    }
    else
    {
#if defined NBD_SET_FLAGS
    flags = 0;
#if defined NBD_FLAG_SEND_TRIM
    flags |= NBD_FLAG_SEND_TRIM;
#endif
#if defined NBD_FLAG_SEND_FLUSH
    flags |= NBD_FLAG_SEND_FLUSH;
#endif
    if (flags != 0 && -1 == ioctl_wrapper(m_dev_fd, NBD_SET_FLAGS, flags))
    {
        m_logger->log(Logger::LOG_LEVEL::ERROR,
                        "ioctl(nbd, NBD_SET_FLAGS, flags) failed",
                        __FILE__,
                        __LINE__,
                        true);
        throw NBDDriverError("ioctl(nbd, NBD_SET_FLAGS, flags) failed");
    }
#endif
        if (-1 == ioctl_wrapper(m_dev_fd, NBD_DO_IT))
        {
            m_logger->log(Logger::LOG_LEVEL::ERROR,
                        "ioctl_wrapper(m_dev_fd, NBD_DO_IT) failed",
                        __FILE__,
                        __LINE__,
                        true);
            throw NBDDriverError("ioctl_wrapper(m_dev_fd, NBD_DO_IT) failed");
        }
    }
    CleanUp();
}  


void NBDDriverCommunicator::CleanUp()
{
        if (-1 == ioctl_wrapper(m_dev_fd, NBD_CLEAR_QUE)) 
        {
            m_logger->log(Logger::LOG_LEVEL::ERROR,
                        "failed to perform nbd cleanup actions",
                        __FILE__,
                        __LINE__,
                        true);
            throw NBDDriverError("ioctl(m_dev_fd, NBD_CLEAR_QUE) failed - CLEANUP");
            
        }
        if (-1 == ioctl(m_dev_fd, NBD_CLEAR_SOCK))
        {
            m_logger->log(Logger::LOG_LEVEL::ERROR,
                        "failed to perform nbd cleanup actions",
                        __FILE__,
                        __LINE__,
                        true);
            throw NBDDriverError("ioctl(m_dev_fd, NBD_CLEAR_SOCK) failed - CLEANUP");
        }
}



static void BlockAllSignals()
{
    sigset_t sigset;
    if (
        sigfillset(&sigset) != 0 ||
        pthread_sigmask(SIG_SETMASK, &sigset, NULL) != 0)
    {
        warn("failed to block signals in child");
        // exit(EXIT_FAILURE);
    }
    }




static int read_all(int fd, char* buf, size_t count)
{
    int bytes_read;

    while (count > 0)
    {
        bytes_read = read(fd, buf, count);
        // assert(bytes_read > 0);
        buf += bytes_read;
        count -= bytes_read;
    }
    // assert(count == 0);

    return 0;
}

static int write_all(int fd, char* buf, size_t count)
{
    int bytes_written;

    while (count > 0)
    {
        bytes_written = write(fd, buf, count);
        // assert(bytes_written > 0);
        buf += bytes_written;
        count -= bytes_written;
    }
    return bytes_written;
}

template <typename ...Args>
static int ioctl_wrapper(int fd, unsigned long request,Args ...args)
{
    return ioctl(fd, request, args...);
}

u_int64_t ntohll(u_int64_t a)
{
    u_int32_t lo = a & 0xffffffff;
    u_int32_t hi = a >> 32U;
    lo = ntohl(lo);
    hi = ntohl(hi);
    return ((u_int64_t) lo) << 32U | hi;
}

} // namespace close