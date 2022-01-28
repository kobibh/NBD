#include "ITask.hpp"

namespace HRD18
{
ITask::ITask(Priority priority) : m_priority(priority)
{}

ITask::~ITask() = default;

bool ITask::operator<(const ITask& rhs) const
{
    return (m_priority < rhs.m_priority);
}

}