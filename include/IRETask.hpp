/*****************************************************
* File: 	   IRETask.hpp                           *
* Date: 	   04/10/2021							 *
* Description: Task interface API - Command DP       * 
******************************************************/
#ifndef ILRD_HRD18_IRETASK_HPP
#define ILRD_HRD18_IRETASK_HPP

#include "ITask.hpp"

namespace HRD18
{

class IRETask : public ITask
{
public:
    enum Priority
    {
        LOW = 0, MED = 1, HIGH = 2, ADMIN = 3
    };
    explicit IRETask(Priority priority);
    virtual ~IRETask() = 0;
    // may throw run-time error
    virtual void Execute() = 0;
}; // class IRETask

} //namespace HRD18

#endif // ILRD_HRD18_IRETASK_HPP