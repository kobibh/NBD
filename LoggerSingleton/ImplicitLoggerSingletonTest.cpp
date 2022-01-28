/* compile as follow:
gdpp11 implicit_logger_singleton_test.cpp -I../include
-L. -Wl,-rpath=. -lLogger -ldl -o implicit_logger_singleton_test
*/

#include <iostream>
#include <stdlib.h>
#include <dlfcn.h>

#include "Singleton.hpp"
#include "Logger.hpp"
#include "LoggerWrapper.hpp"

using namespace HRD18;

const std::string PATH = "log/log.txt";

void LoadImplicit();
void LoadExplicit();
static void* dlsym_wrapper(void* handle, std::string& function_name);

int main()
{
    Logger *logger = Singleton<Logger>::GetInstance();
    std::cout << "from main:    " << logger << std::endl;

    // LoadExplicit();
    LoadImplicit();
    logger->log(Logger::ERROR, "STAM", __FILE__, __LINE__, true);
    logger->log(Logger::DEBUG, "LOG1", __FILE__, __LINE__, true);
    logger->SetLogLevel(Logger::LOG_LEVEL::DEBUG);
    logger->log(Logger::DEBUG, "LOG2(Log1 is not in the adequate log permission)", __FILE__, __LINE__, true);
    logger->log(Logger::ERROR, "LOG3", __FILE__, __LINE__, true);
}


void LoadImplicit()
{
    foo();
}

void LoadExplicit()
{
    void *handle = dlopen ("./libLogger.so", RTLD_LAZY);
    std::string function_name = "foo";
    Logger *(*fptr)() = 
            reinterpret_cast<Logger *(*)()>(dlsym_wrapper(handle, function_name));
    fptr();  
}

// c++ wrapper
static void* dlsym_wrapper(void* handle, std::string& function_name)
{
    // c function
    return dlsym(handle, function_name.c_str());
}
