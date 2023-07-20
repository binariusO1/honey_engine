#include "log_stream/LogStream.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include "logger/Logger.hpp"
#include "sys/config/Config.hpp"


namespace
{
std::string fixedMsecondsValFormat(int value)
{
    if(value < 10)
    {
        return ("00000" + std::to_string(value));
    }
    if(value < 100)
    {
        return ("0000" + std::to_string(value));
    }
    if(value < 1000)
    {
        return ("000" + std::to_string(value));
    }
    if(value < 10000)
    {
        return ("00" + std::to_string(value));
    }
    if(value < 100000)
    {
        return ('0' + std::to_string(value));
    }
    return std::to_string(value);
}
std::string fixedTimeValFormat(int value)
{
    return value < 10 ? ('0' + std::to_string(value)) : std::to_string(value);
}

std::string getCurrentSeconds()
{
    std::chrono::time_point timePoint = std::chrono::system_clock::now();
    std::chrono::duration timeSinceEpoch =  timePoint.time_since_epoch();
    auto currentSeconds = std::chrono::duration_cast<std::chrono::seconds>(timeSinceEpoch).count();
    return fixedTimeValFormat(currentSeconds%60);
}

std::string getCurrentMilliseconds()
{
    std::chrono::time_point timePoint = std::chrono::system_clock::now();
    std::chrono::duration timeSinceEpoch =  timePoint.time_since_epoch();
    auto currentSeconds = std::chrono::duration_cast<std::chrono::seconds>(timeSinceEpoch).count();
    auto currentMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(timeSinceEpoch).count();
    return fixedMsecondsValFormat(currentMicroseconds - (currentSeconds * 1000000));
}

std::string getCurrentTime()
{
    std::time_t tTime = std::time({});
    std::tm *tm_local = localtime(&tTime);
    auto year = std::to_string((tm_local->tm_year)+1900);
    return year+fixedTimeValFormat((tm_local->tm_mon)+1)+fixedTimeValFormat(tm_local->tm_mday) + 'T' + fixedTimeValFormat(tm_local->tm_hour)+fixedTimeValFormat(tm_local->tm_min);
}

bool couldShowLog(const common::log_stream::LogSeverity severity)
{
    switch (sys::Config::Get().dev.LOG_LEVEL())
    {
    case common::log_stream::LogSeverity::debug:
        if (severity == common::log_stream::LogSeverity::memory)
        {
            return false;
        }
        break;
    case common::log_stream::LogSeverity::info:
        if (severity == common::log_stream::LogSeverity::memory or severity == common::log_stream::LogSeverity::debug)
        {
            return false;
        }
        break;
    case common::log_stream::LogSeverity::warning:
        if (severity == common::log_stream::LogSeverity::memory 
        or severity == common::log_stream::LogSeverity::debug 
        or severity == common::log_stream::LogSeverity::info)
        {
            return false;
        }
        break;
    case common::log_stream::LogSeverity::error:
        if (severity == common::log_stream::LogSeverity::memory 
        or severity == common::log_stream::LogSeverity::debug 
        or severity == common::log_stream::LogSeverity::info 
        or severity == common::log_stream::LogSeverity::warning )
        {
            return false;
        }
        break;
    default:
        break;
    }
    return true;
}
} // namespace

namespace common
{
namespace log_stream
{
template<class LogFunction> 
LogStream<LogFunction>::LogStream(const common::log_stream::LogSeverity logSeverity_, const char *streamFilename_, int streamLine_)
    : stream_type(&buffer)
    ,streamFilename(streamFilename_)
    ,streamLine(streamLine_)
    ,logSeverity(logSeverity_)
{
    if (couldShowLog(logSeverity))
    {
        buildContext(); 
    }
    else
    {
        buffer.blockBuffer();
    }
}

template<class LogFunction> 
void LogStream<LogFunction>::buildContext()
{
    stringstream_type os;
    auto currentTime = getCurrentTime();
    auto currentSeconds = getCurrentSeconds();
    auto currentMilliseconds = getCurrentMilliseconds();
    if (logSeverity == common::log_stream::LogSeverity::error)
    {
        os << "\x1B[31m";
    }
    else if(logSeverity == common::log_stream::LogSeverity::warning)
    {
        os << "\x1B[33m";
    }
    os << std::this_thread::get_id() << "|" << currentTime << ':' << currentSeconds << '.' << currentMilliseconds << ' ' << '/' << toString(logSeverity) << '/' << streamFilename << ':' << streamLine << ' ' << "\033[0m";;
    buffer.setContext(os.str());
}

template class LogStream<Logger>;
} // namespace log
} // namespace common



