#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include "log_stream/LogStream.hpp"

namespace common
{
namespace log_stream
{
enum class LogSeverity;
}

// log to file
/*
template<class charT>
class log_to_file : public basic_log_function<charT>
{
public:
    typename basic_log_function<charT>::result_type operator()(typename basic_log_function<charT>::second_argument_type context, typename basic_log_function<charT>::second_argument_type output)
    {    
		std::basic_ofstream<charT> fs(GetLogfilename().c_str(), std::ios_base::app);
        if (!fs)
            throw std::invalid_argument(("Cannot open filestream for " + GetLogfilename()).c_str());
        else
            fs << context << output;
    }
private:
    const std::string GetLogfilename()
    { 
        return std::string("d:\\temp\\debug.log");
    }
};
*/

class Logger : public log_stream::BasicLog
{
	typedef std::basic_string<char> string_type;

public:
    ~Logger(){};
    typename log_stream::BasicLog::resultType operator()(typename log_stream::BasicLog::contextType context, typename log_stream::BasicLog::messageType output)
    {
      string_type s(context);
      s += output;
      std::cout << s.c_str() << std::endl;
    }
};

} // namespace common

typedef ::common::log_stream::LogStream<::common::Logger> logger;

#define LOG_MEMORY logger(::common::log_stream::LogSeverity::memory, __FILE_NAME__, __LINE__).get()
#define LOG_DEBUG logger(::common::log_stream::LogSeverity::debug, __FILE_NAME__, __LINE__).get()
#define LOG_ERROR logger(::common::log_stream::LogSeverity::error, __FILE_NAME__, __LINE__).get()
#define LOG_INFO logger(::common::log_stream::LogSeverity::info, __FILE_NAME__, __LINE__).get()
#define LOG_WARNING logger(::common::log_stream::LogSeverity::warning, __FILE_NAME__, __LINE__).get()

#define LOG_CONSTRUCTOR "\x1B[32m[constructor] \033[0m"
#define LOG_COPY_CONSTRUCTOR "\x1B[33m[copy constructor] \033[0m"
#define LOG_COPY_ASSIGNED "\x1B[33m[copy constructor] \033[0m"
#define LOG_MOVE_CONSTRUCTOR "\x1B[36m[move] \033[0m"
#define LOG_MOVE_ASSIGNED "\x1B[36m[move] \033[0m"
#define LOG_DESTRUCTOR "\x1B[91m[destructor] \033[0m"

#define LOG_RED "\x1B[31m"
#define LOG_BRIGHT_RED "\x1B[91m"
#define LOG_GREEN "\x1B[32m"
#define LOG_BRIGHT_GREEN "\x1B[92m"
#define LOG_BLUE "\x1B[34m"
#define LOG_BRIGHT_BLUE "\x1B[94m"
#define LOG_WHITE "\033[0m"
#define LOG_YELLOW "\x1B[33m"
#define LOG_LIGHT_YELLOW "\x1B[93m"

#define LOG_THIS_1 LOG_DEBUG << LOG_RED << "THIS <-- (1)" << LOG_WHITE
#define LOG_THIS_2 LOG_DEBUG << LOG_BRIGHT_RED << "THIS <-- (2)" << LOG_WHITE
#define LOG_THIS_3 LOG_DEBUG << LOG_YELLOW << "THIS <-- (3)" << LOG_WHITE
#define LOG_THIS_4 LOG_DEBUG << LOG_LIGHT_YELLOW << "THIS <-- (4)" << LOG_WHITE
#define LOG_THIS_5 LOG_DEBUG << LOG_BRIGHT_GREEN << "THIS <-- (5)" << LOG_WHITE
#define LOG_THIS_6 LOG_DEBUG << KIG_GREEN << "THIS <-- (6)" << LOG_WHITE
#define LOG_THIS_7 LOG_DEBUG << LOG_BLUE << "THIS <-- (7)" << LOG_WHITE
#define LOG_THIS_8 LOG_DEBUG << LOG_BRIGHT_BLUE << "THIS <-- (8)" << LOG_WHITE

/*
Name            FG  BG
Black           30  40
Red             31  41
Green           32  42
Yellow          33  43
Blue            34  44
Magenta         35  45
Cyan            36  46
White           37  47
Bright Black    90  100
Bright Red      91  101
Bright Green    92  102
Bright Yellow   93  103
Bright Blue     94  104
Bright Magenta  95  105
Bright Cyan     96  106
Bright White    97  107

// Examples:
// printf("\x1B[31mTexting\033[0m\t\t");
// printf("\033[3;42;30mTexting\033[0m\t\t");
*/
