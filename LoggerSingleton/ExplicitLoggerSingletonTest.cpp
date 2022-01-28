/* Description: Example of loading singleton in shared object function
   Observations: 1. Loading it with dlsym will generate new singleton instance because
                     the symbol isnt exist
                 2. compile this exe with `-rdynamic` flag will expose this symbol and
                     will use the same instance */

/*compile as follows :
gdpp11 explicit_logger_singleton_test.cpp Logger.cpp -I../include -ldl -rdynamic -o explicit_logger_singleton_test 
----- compiling without -rdynamic will generate different instances ----- */

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
    LoadExplicit();
    // LoadImplicit();
    logger->log(Logger::ERROR, "STAM", PATH, __LINE__);
    logger->log(Logger::DEBUG, "STAM", PATH, __LINE__);
}


void LoadImplicit()
{
    // foo();
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
