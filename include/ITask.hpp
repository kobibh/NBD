/*****************************************************
* File: 	   itask.hpp                          	 *
* Date: 	   04/10/2021							 *
* Description: Task interface API - Command DP        * 
******************************************************/

#ifndef ILRD_HRD18_ITASK
#define ILRD_HRD18_ITASK


namespace HRD18
{

class ITask
{
public:
    // ADMIN level is only for implementation, should not be used
    enum Priority
    {
        STOP = -1, LOW = 0, MED = 1, HIGH = 2, ADMIN = 3
    };

    explicit ITask(Priority priority);
    virtual ~ITask() = 0;
    ITask(const ITask& other) = delete;
    ITask& operator=(const ITask& rhs) = delete;

    bool operator<(const ITask& rhs) const;
private:
    Priority m_priority;
    // may throw run-time error
    virtual void Execute() = 0;
    friend class ThreadPool;


};



} // namespace HRD18

#endif // ILRD_HRD18_ITASK