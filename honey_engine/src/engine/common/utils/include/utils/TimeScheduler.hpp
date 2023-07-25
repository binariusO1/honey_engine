#pragma once

#include <condition_variable>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <thread>
#include "logger/Logger.hpp"
#include "utils/WorkerThread.hpp"

namespace utils
{
using sys_clock = std::chrono::_V2::system_clock;
using time_pt = std::chrono::time_point<sys_clock, sys_clock::duration>;

using TimeSinceEpoch = std::uint64_t;
using TimerCallback = std::function<void()>;
using TimersMap = std::multimap<std::uint64_t, std::uint64_t>;
using CallbacksMap = std::unordered_map<std::uint64_t, TimerCallback>;

class TimeScheduler
{
private:
    TimeScheduler();
    ~TimeScheduler();

public:
    static TimeScheduler& Get()
    {
        static TimeScheduler instance;
        return instance;
    }

public:
    TimeScheduler(TimeScheduler&) = delete;
    void operator=(const TimeScheduler&) = delete;

    void addTimer(std::uint64_t id, unsigned int timeoutTime, std::function<void()> callback);
    void stopTimer(std::uint64_t id);
    void start();
    void stop();

private:
    void private_start();

private:
    std::condition_variable m_condition;
    TimersMap m_timers{};
    CallbacksMap m_callbacks{};
    std::mutex m_mutex;
    std::thread m_thread;
    bool m_running{false};
    bool m_threadActive{false};
    Worker m_worker;
};
} // namespace utils

