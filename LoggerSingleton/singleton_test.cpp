
#include <iostream>

#include "Logger.hpp"
#include "Singleton.hpp"

using namespace HRD18;

const std::string PATH = "../log/log.txt";

int main()
{
    Logger *logger = Singleton<Logger>::GetInstance();
    Logger *logger2 = Singleton<Logger>::GetInstance();
    Logger *logger3 = Singleton<Logger>::GetInstance();
    
    std::cout << logger << "    " << logger2 << "    " << logger3 << std::endl;
    
    logger->log(Logger::ERROR, "STAM", PATH, __LINE__);
}