#ifndef ILRD_HRD18_TASKIMP
#define ILRD_HRD18_TASKIMP
 
#include "ITask.hpp"

namespace HRD18
{

class TaskImp : public ITask
{
public:
    TaskImp(ITask::Priority priority);
    ~TaskImp();

    virtual void Execute();
};

}


#endif // ILRD_HRD18_TASKIMP