#pragma once

#include <sstream>
#include <streambuf>

namespace common
{
namespace log_stream
{

using traits = std::char_traits<char>;
using string_type = std::basic_string<char, log_stream::traits>;
using stream_type = std::basic_ostream<char, log_stream::traits> ;
using stringstream_type = std::basic_ostringstream<char, log_stream::traits> ;

struct BasicLog 
{
    typedef void resultType;
    typedef const char * const contextType;
    typedef const char * const messageType;
};

template<class LogFunction>
class LogBuffer : public std::basic_streambuf<char, log_stream::traits>
{
public:
    virtual ~LogBuffer()
    {
        sync();         // flush buffer
    }
    void setContext(const log_stream::string_type &context)
    {
        buffContext = context;
    }
    void blockBuffer()
    {
        m_block = true;
    }

protected:
    virtual typename log_stream::traits::int_type overflow(typename log_stream::traits::int_type shiftedCharacter)
    {
        if (not log_stream::traits::eq_int_type(shiftedCharacter, log_stream::traits::eof()))
        {
            buffer += log_stream::traits::to_char_type(shiftedCharacter);
            if (shiftedCharacter == log_stream::traits::to_int_type('\n'))
            {
                sync(); 	
            }
        }
        return log_stream::traits::not_eof(shiftedCharacter);              
    }
    virtual int sync()
    {
        if (not buffer.empty() and not m_block)
        {
            sendToDebugLog();
        }
        m_block = false;
        buffer.clear();
        return 0;
    }

private:
    log_stream::string_type buffer;
    log_stream::string_type buffContext;
	LogFunction logFunction;
    bool m_block{false};

    void sendToDebugLog()
    {
        logFunction(buffContext.c_str(), buffer.c_str()); 
    }
};

} // namespace log
} // namespace common
