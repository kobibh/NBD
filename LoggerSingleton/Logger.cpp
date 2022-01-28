#include <fstream>
#include <ctime>
#include <thread>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <string>

#include "Logger.hpp"

namespace HRD18
{
static const char* GetTime();
static const char* ConvertLevelToStringLevel(Logger::LOG_LEVEL level);

Logger::Logger() : m_fstream(), m_log_level(), m_mutex()
{
    m_fstream.open("log/log.txt", std::fstream::out | std::fstream::app);
}

void Logger::log(Logger::LOG_LEVEL level, const std::string& msg, const std::string& file, size_t line, bool print_to_screen)
{
    if (m_log_level < level)
    {
        return;
    }

    if (print_to_screen)
    {
        std::cout << msg << std::endl;
    }
      
    std::stringstream log_line;
    std::string date_str(GetTime());
    //delete the '/n' from string *
    date_str.erase(std::remove(date_str.begin(), date_str.end(), '\n'), date_str.end());
    std::lock_guard<std::mutex> lock(m_mutex);
    m_fstream << ConvertLevelToStringLevel(level)
             << ", "
             << date_str
             << ", Thread_id ="
             << std::this_thread::get_id()
             << ", file "
             << file
             << ", line "
             << line
             << ", "
             << msg
             << std::endl;
}

void Logger::SetLogLevel(LOG_LEVEL newLevel)
{
    m_log_level = newLevel;
}


static const char* GetTime()
{
    time_t now = time(0);
    char *dt = ctime(&now);

    return dt;
}

static const char* ConvertLevelToStringLevel(Logger::LOG_LEVEL level)
{
    switch (level)
    {
    case Logger::LOG_LEVEL::ERROR:
        return "Error";
        break;
    case Logger::LOG_LEVEL::INFO:
        return "Info";
        break;
    case Logger::LOG_LEVEL::DEBUG:
        return "Debug";
        break;
    }
    return NULL;
}

}