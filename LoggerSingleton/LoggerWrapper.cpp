#include <iostream>
#include <dlfcn.h> // dlsym, dlopen
#include "Logger.hpp"
#include "Singleton.hpp"
#include "LoggerWrapper.hpp"
using namespace HRD18;


extern "C"
{

void foo()
{
    Logger *logger = Singleton<Logger>::GetInstance();
    std::cout << "from so:      " << logger << std::endl;
}
}

