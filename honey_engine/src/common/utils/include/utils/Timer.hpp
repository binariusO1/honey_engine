#pragma once

#include <chrono>
#include <mutex>
#include <string>
#include <thread>
#include "logger/Logger.hpp"
#include "utils/TimeScheduler.hpp"
#include "utils/Chronometer.hpp"

namespace utils
{
enum class TimerType : std::uint8_t
{
    Button,
    Default
};

class Timer
{
public:
    Timer()
    {
        m_id = getUniqueId();
    }

    Timer(const TimerType type)
    {
        if (type != TimerType::Default)
        {
            m_id = static_cast<std::uint64_t>(type);
        }
        else
        {
            m_id = getUniqueId();
            m_debugLogs = false;
        }
    }

    ~Timer() 
    { 
        LOG_MEMORY << LOG_DESTRUCTOR;
        //stop(); 
    }

    template<typename Function>
    void setTimeout(Function function, unsigned int duration)
    {
        m_timeoutTime = duration;
        TimeScheduler::Get().addTimer( m_id, duration, [id = m_id, function](){
            function();
            if (id > static_cast<std::uint64_t>(TimerType::Default))
            {
                LOG_DEBUG << LOG_BRIGHT_RED << "Timer: " << id << " Timeout! " << LOG_WHITE;
            }

        });
    }

    //template<typename Function>
    //void setInterval(Function function, std::uint32_t interval);
    void stop()
    {
        TimeScheduler::Get().stopTimer(m_id);
    }

public:
    std::uint64_t getId() const
    {
        return m_id;
    }

    unsigned int getTimeoutTime() const
    {
        return m_timeoutTime;
    }

private:
    std::uint64_t getUniqueId()
    {
        std::scoped_lock lock(m_idMutex);
        static std::uint64_t id = static_cast<std::uint64_t>(TimerType::Default);
        return id++;
    }

private:
     unsigned int m_timeoutTime;
     bool m_debugLogs{true};
     std::uint64_t m_id;
     std::recursive_mutex m_idMutex;
};
} // namespace utils

