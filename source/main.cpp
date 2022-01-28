#include <iostream>
#include <unistd.h>
#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <csignal>
#include <fcntl.h>

#include "NBDDriverCommunicator.hpp"
#include "RAMStorage.hpp"
#include "Logger.hpp"
#include "Singleton.hpp"

using namespace HRD18;
#ifndef BUSE_DEBUG
  #define BUSE_DEBUG (0)
#endif

const size_t drive_size = 134217728;
const size_t chunk_size = 10;


static void SetRequestHandlerSignals(sigset_t* set);
static int EpollCreate1Wrapper();
static Logger* GetLogger();

volatile int IsRunning = 1;
int main(int argc, char *argv[])
{
    if (1 > argc)
    {
      std::cout << "Check parameters..." <<std::endl;
    }
    Logger *logger = GetLogger();
    logger->SetLogLevel(Logger::LOG_LEVEL::DEBUG);
    NBDDriverCommunicator nbd_driver(std::string(argv[1]), drive_size);
    auto sig_handler = [](int sig_num)
                       {
                        (void)sig_num; //unused;
                        IsRunning = 0;
                       };
    std::signal(SIGINT, sig_handler);
    std::signal(SIGTERM, sig_handler);
    RAMStorage storage(drive_size);
    std::cout << "main loop starts..." << std::endl;
    sigset_t set;
    try
    {
      SetRequestHandlerSignals(&set);
    }
    catch(const std::exception& e)
    {
      logger->log(Logger::LOG_LEVEL::ERROR, e.what(), __FILE__, __LINE__, true);
    }
    

    // TODO: handle throw
    int epoll_fd = EpollCreate1Wrapper();
    fcntl(nbd_driver.GetRequestFD(), F_SETFL, O_NONBLOCK);

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = epoll_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, nbd_driver.GetRequestFD(), &event);

    while (IsRunning)
    {
      std::cout << "epoll_wait thread number" << std::this_thread::get_id() << std::endl;
      
      int events_num = epoll_pwait(epoll_fd, &event, 1, 5000, &set);
      for (int i = 0; i< events_num; ++i)
      {
        std::shared_ptr<DriverData> request = nbd_driver.RetrieveRequest();
        request->m_status = 0;
        
        switch(request->m_type) 
        {
          case DriverData::READ:
            /* Fill with zero in case actual read is not implemented */
            std::cout << "Request for read of size " << request.get()->m_len << std::endl;
            storage.Read(request);
            break;
          case DriverData::WRITE:
            std::cout << "Request for write of size " << request.get()->m_len << std::endl;
            storage.Write(request);
            break;
          case DriverData::FLUSH:
            std::cout << "Got NBD_CMD_FLUSH" << std::endl;
            break;
          case DriverData::TRIM:
            std::cout << "Got NBD_CMD_TRIM" << std::endl;
            break;
          case DriverData::DISC:
            std::cout << "Got NBD_CMD_DISC" << std::endl;
            break;
        }   
        nbd_driver.SendReply(request);
      }
    }

    return (0);
}


static void SetRequestHandlerSignals(sigset_t* set)
{
    if (
    sigemptyset(set) != 0 ||
    sigaddset(set, SIGINT) != 0 ||
    sigaddset(set, SIGTERM) != 0
    ) 
    {
      throw std::runtime_error("failed to add signal to set");
    }
}

static int EpollCreate1Wrapper()
{
  int fd;
  if(-1 == (fd = epoll_create1(0)))
  {
    throw;
  }
  return fd;
}

static Logger* GetLogger()
{
  return Singleton<Logger>::GetInstance();
}