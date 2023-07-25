# pragma once

#include <cstdint>
#include <optional>
#include <ostream>

namespace he
{
namespace common
{
using TaskId = std::uint64_t;
enum class ETaskResult : std::uint8_t
{
    eTaskResultOk = 0,
    eTaskResultNok,
    eTaskResultTimeout,
    eTaskResultNokAndInternalCancelTriggered,
    eTaskResultException,
    eTaskResultAbort
};

struct TaskDone
{
    TaskDone(const TaskId& taskId, const ETaskResult& taskResult) : taskId(taskId), taskResult(taskResult) {}
    
    friend bool operator==(const TaskDone& lhs, const TaskDone& rhs)
    {
        return (lhs.taskId == rhs.taskId) and (lhs.taskResult == rhs.taskResult);
    }

    TaskId taskId;
    ETaskResult taskResult;
};

class ITaskMaster
{
public:
    virtual ~ITaskMaster() = default;
    virtual void process_event(const TaskDone&) = 0;
};

inline std::ostream& operator<<(std::ostream& os, const ETaskResult& eTaskResult)
{
    switch (eTaskResult)
    {
        case ETaskResult::eTaskResultOk:
            return os << "eTaskResultOk";
        case ETaskResult::eTaskResultNok:
            return os << "eTaskResultNok";
        case ETaskResult::eTaskResultTimeout:
            return os << "eTaskResultTimeout";
        case ETaskResult::eTaskResultNokAndInternalCancelTriggered:
            return os << "eTaskResultNokAndInternalCancelTriggered";
        case ETaskResult::eTaskResultException:
            return os << "eTaskResultException";
        case ETaskResult::eTaskResultAbort:
            return os << "eTaskResultAbort";
        default:
            return os << "Unexpected eTaskResult" << std::to_string(static_cast<int>(eTaskResult))
    }
}
} // namespace common
} // namespace he