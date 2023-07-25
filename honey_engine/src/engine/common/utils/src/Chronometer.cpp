#include "logger/Logger.hpp"
#include "utils/Chronometer.hpp"

#include <algorithm>
#include <string>
namespace
{
////////////////////////////////////////////////////////////
std::string timeFormat(const std::chrono::microseconds& durationTime)
{
    const int uS_UN_SEC = 1000000;

    std::string output = "";
    std::string unit = "μs";

    auto timeInUs = std::chrono::duration_cast<std::chrono::microseconds>(durationTime).count();
    auto timeInS = std::chrono::duration_cast<std::chrono::seconds>(durationTime).count();
    auto timeInMin = std::chrono::duration_cast<std::chrono::minutes>(durationTime).count();
    auto timeInHour = std::chrono::duration_cast<std::chrono::hours>(durationTime).count();

    if (timeInHour > 0)
    {
        output += std::to_string(timeInHour);
        output += "h ";
        unit = "hours";
    }
    if (timeInMin > 0)
    {
        timeInMin %= 60;
        if (timeInMin < 10)
        {
            output += '0';
        }
        output += std::to_string(timeInMin);
        output += "min ";
        unit = "min";
    }
    if (timeInS > 0)
    {
        timeInMin %= 60*uS_UN_SEC;
        if ((timeInS < 10) and (timeInMin = 0) and (timeInHour = 0))
        {
            output += '0';
        }
        output += std::to_string(timeInS);
        output += '.';
        unit = 's';
    }
    std::string usString=std::to_string(timeInUs%=uS_UN_SEC);
    output += std::string(std::min(6-usString.length(), usString.length()), '0') + usString;

    return output + ' ' + unit +" total";
}
} // namespace

namespace utils
{
////////////////////////////////////////////////////////////
Chronometer::Chronometer() : m_startTime{std::chrono::high_resolution_clock::now()}
{
    LOG_MEMORY << LOG_CONSTRUCTOR;
    m_id = id++;
}


////////////////////////////////////////////////////////////
Chronometer::Chronometer(const std::string& ChronometerName) : m_ChronometerName{ChronometerName}
{
    LOG_MEMORY << LOG_CONSTRUCTOR;
    m_id = id++;
}


////////////////////////////////////////////////////////////
Chronometer::Chronometer(Chronometer& copy) 
    : m_startTime{copy.m_endTime}
    , m_endTime{copy.m_endTime}
    , m_isRunning{copy.m_isRunning}
    , m_ChronometerName{copy.m_ChronometerName}
    , m_id{++id}
{
    LOG_MEMORY << LOG_COPY_CONSTRUCTOR;
}


//////////////////////////////////////////////////////////////////////
Chronometer::~Chronometer()
{
    LOG_MEMORY << LOG_DESTRUCTOR;
}


////////////////////////////////////////////////////////////
void Chronometer::start()
{
    if (m_extendedLogs)
    {
        LOG_DEBUG << "Start Chronometer id: " << m_id;
    }
    if (not m_isRunning)
    {
        m_startTime = std::chrono::high_resolution_clock::now();
        m_endTime = std::chrono::high_resolution_clock::now();
        m_isRunning = true;
    }
}


////////////////////////////////////////////////////////////
void Chronometer::end()
{
    if (m_extendedLogs)
    {
        LOG_DEBUG<< "End Chronometer id: " << m_id;
    }
    m_endTime = std::chrono::high_resolution_clock::now();
    m_isRunning = false;
}


////////////////////////////////////////////////////////////
void Chronometer::reset()
{
    m_endTime = std::chrono::high_resolution_clock::now();
    m_startTime = m_endTime;
    if (not m_isRunning)
    {
        m_isRunning = true;
    }
}


////////////////////////////////////////////////////////////
void Chronometer::setOverflow(std::uint64_t value)
{
    m_overflow = std::chrono::microseconds{value};
}


////////////////////////////////////////////////////////////
void Chronometer::printElapsedTime()
{
    auto elapsedTime = getElapsedTime();
    auto strElapsedTime = timeFormat(elapsedTime);
    bool overflow{false};
    if (m_overflow > std::chrono::microseconds{0} and elapsedTime > m_overflow)
    {
        overflow = true;
    }

    if (m_ChronometerName != "")
    {
        LOG_DEBUG << (overflow ? LOG_RED : "") << "Chronometer id: " << m_id << " (" << m_ChronometerName << ") elapsed time: " << strElapsedTime << LOG_WHITE;
    }
    else
    {
        LOG_DEBUG << (overflow ? LOG_RED : "") << "Chronometer id: " << m_id << " elapsed time: " << strElapsedTime << LOG_WHITE;
    }

}


////////////////////////////////////////////////////////////
void Chronometer::printOnlyOverflow()
{
    auto elapsedTime = getElapsedTime();
    auto strElapsedTime = timeFormat(elapsedTime);
    if (m_overflow > std::chrono::microseconds{0} and elapsedTime > m_overflow)
    {
        if (m_ChronometerName != "")
        {
            LOG_DEBUG << LOG_YELLOW << "Chronometer id: " << m_id << " (" << m_ChronometerName << ") elapsed time: " << strElapsedTime << LOG_WHITE;
        }
        else
        {
            LOG_DEBUG << LOG_YELLOW << "Chronometer id: " << m_id << " elapsed time: " << strElapsedTime << LOG_WHITE;
        }
    }
    else if (m_overflow == std::chrono::microseconds{0})
    {
        LOG_WARNING << "No overflow specified! Set overflow fist.";
    }
}


////////////////////////////////////////////////////////////
std::chrono::microseconds Chronometer::getElapsedTime()
{
    if (not m_isRunning)
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(m_endTime - m_startTime);
    }
    auto currentTime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(currentTime - m_startTime);
}


////////////////////////////////////////////////////////////
void Chronometer::extendLogs(bool state)
{
    m_extendedLogs = state;
}


inline std::uint64_t Chronometer::id;
} // namespace utils