#pragma once

#include <iostream>
#include "log_stream/LogBuffer.hpp"
#include "log_stream/LogSeverity.hpp"

namespace common
{
namespace log_stream
{
template<class LogFunction>
class LogStream : public std::basic_ostream<char, log_stream::traits>
{
public:
    LogStream(const LogSeverity, const char *streamFilename, int streamLine);
    LogStream &get() {return *this;}

    virtual ~LogStream(){}

private:
    void buildContext();
    LogStream(const LogStream &);
    LogStream &operator=(const LogStream &);

    using buffer_type = LogBuffer<LogFunction>;

    const char *streamFilename;
    const int streamLine;
    buffer_type buffer;

    LogSeverity logSeverity;
};

} // namespace log
} // namespace common