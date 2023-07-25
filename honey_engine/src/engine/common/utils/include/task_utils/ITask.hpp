#pragma once

#include "task_utils/ITaskMaster.hpp"

namespace he
{
namespace common
{
struct StartTask
{
    StartTask(const TaskId& taskId, ITaskMaster& taskmaster) : taskId(taskId), taskmaster(taskmaster) {}

    friend bool operator==(const StartTask& lhs, const StartTask& rhs)
    {
        return (lhs.taskId == rhs.taskId) and (std::addressof(lhs.taskmaster) == std::addressof(rhs.taskmaster));
    }    

    TaskId taskId;
    ITaskMaster& taskmaster;
};

class ITask
{
public:
    virtual ~ITask() = default;
    virtual void process_event(const StartTask&) = 0;
    virtual void abort(){};
};
} // namespace common
} // namespace he