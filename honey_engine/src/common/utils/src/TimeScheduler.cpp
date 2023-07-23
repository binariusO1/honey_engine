#include "utils/TimeScheduler.hpp"
#include "utils/Chronometer.hpp"

namespace utils
{
////////////////////////////////////////////////////////////
TimeScheduler::TimeScheduler() = default;


////////////////////////////////////////////////////////////
TimeScheduler::~TimeScheduler()
{ 
    LOG_MEMORY << LOG_DESTRUCTOR;
    stop(); 
}


////////////////////////////////////////////////////////////
void TimeScheduler::addTimer(std::uint64_t id, unsigned int timeoutTimer, std::function<void()> callback)
{
    utils::Chronometer chronos;
    chronos.start();
    auto timeNow = std::chrono::high_resolution_clock::now();
    auto timeoutTime = timeNow + std::chrono::milliseconds{timeoutTimer};
    auto timeout = timeoutTime.time_since_epoch().count();

    // PERFORMANCE : BOTTLENECK
    if (m_callbacks.emplace(id, callback).second)
    {
        m_timers.emplace(timeout, id); 
    }
    else
    {
        for (auto& iter : m_timers)
        {
            if (iter.second == id)
            {
                m_timers.erase(iter.first);
                m_timers.emplace(timeout, id);
                break;
            }
        }
    }
    m_running = true;
    m_condition.notify_one();
    chronos.end();
    //LOG_DEBUG << "m_timers.size(): " << m_timers.size();
    //chronos.printElapsedTime();
}


////////////////////////////////////////////////////////////
void TimeScheduler::stopTimer(std::uint64_t id)
{
    m_callbacks.erase(id);
    for (auto& iter : m_timers)
    {
        if (iter.second == id)
        {
            m_timers.erase(iter.first);
            break;
        }
    }
}


////////////////////////////////////////////////////////////
void TimeScheduler::start()
{
    {
        if (m_threadActive)
        {
            return;
        }
    }
    m_worker.start();
    private_start();
}


////////////////////////////////////////////////////////////
void TimeScheduler::stop()
{
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        if (not m_threadActive)
        {
            return;
        }
        m_threadActive = false;
    }
    m_running = true;
    m_condition.notify_all();
    m_thread.join();
}


////////////////////////////////////////////////////////////
void TimeScheduler::private_start()
{
    m_thread = std::thread([this]
    {
        m_threadActive = true;
        // LOG_DEBUG << LOG_BRIGHT_BLUE << "Start timers_thread" << LOG_WHITE;
        while (m_threadActive)
        {
            std::unique_lock lk(m_mutex);
            m_condition.wait(lk, [this]{
                if (not m_running)
                {
                    // LOG_DEBUG << LOG_BRIGHT_BLUE << "Ready for notify" << LOG_WHITE;
                }
                return m_running; });
            lk.unlock();
            std::this_thread::sleep_for(std::chrono::microseconds(25));

            std::uint64_t timeNow = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            for (auto const& [time, id] : m_timers)
            {
                if (time <= timeNow)
                {
                    m_worker.push_task(m_callbacks[id]);
                    // LOG_DEBUG << LOG_BRIGHT_BLUE << "timer timeout! " << LOG_WHITE;
                    m_timers.erase(time);
                    m_callbacks.erase(id);
                }
                else
                {
                    break;
                }
            }
            if (m_timers.empty())
            {
                m_running = false;
                m_condition.notify_one();
            }
        }
        // LOG_DEBUG << LOG_BRIGHT_BLUE << "timers_thread end working" << LOG_WHITE;
    });
    m_threadActive = false;
}
} // namespace utils