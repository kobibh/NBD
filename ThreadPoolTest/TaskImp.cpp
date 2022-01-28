
#include <iostream>
#include "TaskImp.hpp"


namespace HRD18
{


TaskImp::TaskImp(ITask::Priority priority) : ITask(priority)
{}

TaskImp::~TaskImp()
{}

void TaskImp::Execute()
{
    std::cout << "Execute" << std::endl;
}


}