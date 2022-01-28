#include <iostream>

#include "ThreadPool.hpp"
#include "TaskImp.hpp"

using namespace HRD18;

int main()
{
    ThreadPool tp;
    
    std::shared_ptr<TaskImp> task1 = std::make_shared<TaskImp>(TaskImp::Priority::LOW);
    std::shared_ptr<TaskImp> task2 = std::make_shared<TaskImp>(TaskImp::Priority::LOW);
    std::shared_ptr<TaskImp> task3 = std::make_shared<TaskImp>(TaskImp::Priority::LOW);
    std::shared_ptr<TaskImp> task4 = std::make_shared<TaskImp>(TaskImp::Priority::HIGH);
    std::shared_ptr<TaskImp> task5 = std::make_shared<TaskImp>(TaskImp::Priority::MED);
    

    tp.AddTask(task1);
    tp.AddTask(task2);
    tp.AddTask(task3);
    tp.AddTask(task4);
    tp.AddTask(task5);

    tp.SetSize(2);
    tp.SetSize(5);
    
    
    return (0);
}




