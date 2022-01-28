#include "IDriverCommunicator.hpp"

namespace HRD18
{

DriverError::DriverError(const std::string& msg) : std::runtime_error(msg)  
{}

}