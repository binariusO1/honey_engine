#pragma once

#include <chrono>
#include "utils/Chronometer.hpp"

namespace he
{
namespace window
{
class FrameGuard
{
public:
    FrameGuard() = default;
    ~FrameGuard() = default;

    void guardFrame();
    void setFramerateLimit(unsigned int limit);
    void startFrameGuard();
    void endFrameGuard();


private:
    std::chrono::microseconds m_frameTimeLimit{0};
    utils::Chronometer m_Chronometer;
    std::uint16_t m_frames{0};

    const std::chrono::microseconds SECOND{1000000};
};
} // namespace he
} // namespace window