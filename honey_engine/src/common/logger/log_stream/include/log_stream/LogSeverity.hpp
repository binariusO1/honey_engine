#pragma once

namespace common
{
namespace log_stream
{
enum class LogSeverity
{
    memory,
    debug,
    error,
    info,
    warning
};

inline const char* toString(const LogSeverity& logSeverity)
{
    switch(logSeverity)
    {
        case LogSeverity::memory: return "MEM";
        case LogSeverity::debug: return "DBG";
        case LogSeverity::error: return "ERR";
        case LogSeverity::info: return "INF";
        case LogSeverity::warning: return "WRN";
        default: return "none";
    }
}
} // namespace log
} // namespace common