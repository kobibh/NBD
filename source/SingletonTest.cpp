#include <iostream>
#include <stdlib.h>
#include <dlfcn.h>

#include "Logger.hpp"
#include "Singleton.hpp"

using namespace HRD18;

static void* dlsym_wrapper(void* handle, std::string& function_name);

const std::string PATH = "log/log.txt";

int main()
{
    Logger *logger = Singleton<Logger>::GetInstance();
    Logger *logger2 = Singleton<Logger>::GetInstance();
    Logger *logger3 = Singleton<Logger>::GetInstance();
    
    // void *handle = dlopen ("./libLogger.so", RTLD_LAZY);
    // std::string function_name = "Singleton<Logger>::GetInstance";
    // Logger *(*fptr)() = 
    //         reinterpret_cast<Logger *(*)()>(dlsym_wrapper(handle, function_name));

    // Logger *logger4 = fptr();
    // Logger *logger5 = fptr();
    std::cout << logger << "    " << logger2 << "    " << logger3 << "    " << std::endl;
    // logger4->log(Logger::ERROR, "STAM", PATH, __LINE__);
    // logger5->log(Logger::ERROR, "STAM", PATH, __LINE__);
}

// c++ wrapper
static void* dlsym_wrapper(void* handle, std::string& function_name)
{
    // c function
    return dlsym(handle, function_name.c_str());
}