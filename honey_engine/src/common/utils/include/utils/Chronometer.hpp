#pragma once

#include <chrono>
#include <memory>
#include <string>

namespace utils
{
using sys_clock = std::chrono::_V2::system_clock;
using time_pt = std::chrono::time_point<sys_clock, sys_clock::duration>;
class Chronometer
{
public:
    Chronometer();
    Chronometer(const std::string& ChronometerName);
    Chronometer(Chronometer&);
    ~Chronometer();

public:
    void start(); // TODO - dodać log, ze zaczynam start, oraz dodac unique id Chronometera
    void end();
    void reset();
    void printElapsedTime();
    void printOnlyOverflow();
    void setOverflow(std::uint64_t);
    std::chrono::microseconds getElapsedTime();
    void extendLogs(bool);

private:
    time_pt m_startTime;
    time_pt m_endTime;
    bool m_isRunning{false};
    std::uint64_t m_id;
    std::chrono::microseconds m_overflow;
    const std::string m_ChronometerName;
    bool m_extendedLogs{false};

private:
    static std::uint64_t id;
};
} // namespace utils

