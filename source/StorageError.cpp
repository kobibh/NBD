#include "IStorage.hpp"

namespace HRD18
{

StorageError::StorageError(const std::string& msg) : std::runtime_error(msg)  
{}

}