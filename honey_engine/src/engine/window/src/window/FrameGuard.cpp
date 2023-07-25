#include "window/window/FrameGuard.hpp"
#include "logger/Logger.hpp"

namespace he
{
namespace window
{
////////////////////////////////////////////////////////////
void FrameGuard::startFrameGuard()
{
    m_Chronometer.start();
    m_frames = 0;
}


////////////////////////////////////////////////////////////
void FrameGuard::endFrameGuard()
{
    m_Chronometer.end();
    m_frames = 0;
}


////////////////////////////////////////////////////////////
void FrameGuard::guardFrame()
{
    m_frames++;

    if (m_Chronometer.getElapsedTime() >= SECOND)
    {
        auto realFrameTime = SECOND / m_frames;
        if (realFrameTime > m_frameTimeLimit)
        {
            LOG_ERROR << "Framerate time limit (" << m_frameTimeLimit.count() << ") has been exceeded! Framerate: " << realFrameTime.count() << " (" << m_frames << ") frames";
        }
        // LOG_DEBUG << "Real frame time: " << realFrameTime << " , frames:" << m_frames;
        m_frames = 0;
        m_Chronometer.reset();
    }
}


////////////////////////////////////////////////////////////
void FrameGuard::setFramerateLimit(unsigned int limit)
{
    if (limit > 0)
    {
        m_frameTimeLimit = static_cast<std::chrono::microseconds>(SECOND / limit);
        LOG_INFO << "Set framerate limit: " << m_frameTimeLimit.count();
    }
    else
    {
        m_frameTimeLimit = std::chrono::microseconds{0};
    }
}
} // namespace window
} // namespace he