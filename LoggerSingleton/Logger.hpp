/***
* File: logger.hpp
* Date: 26/09/2021
* Logger has 3 levels:
1. Error - error messages only
2. Info - include error and info messages
3. Debug - include all: error, info and debug messages 
***/
#ifndef ILRD_HRD18_LOGGER
#define ILRD_HRD18_LOGGER

#include <string>
#include <fstream>
#include "Singleton.hpp"
namespace HRD18
{
class Logger
{
public:
    ~Logger() = default;
    enum LOG_LEVEL 
    {
        ERROR, INFO, DEBUG
    };
    void log(LOG_LEVEL level, const std::string& msg, const std::string& path, size_t line, bool print_to_screen = false);
    void SetLogLevel(LOG_LEVEL newLevel);
private:
    friend class Singleton<Logger>;
    explicit Logger();
    std::ofstream m_fstream;
    LOG_LEVEL m_log_level;
    std::mutex m_mutex;
};
#endif // ILRD_HRD18_LOGGER
}