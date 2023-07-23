#include "utils/WorkerThread.hpp"

#include "logger/Logger.hpp"

namespace utils
{
////////////////////////////////////////////////////////////
Worker::Worker(bool start) : m_Running(start) 
{
    if (start) 
    {
        private_start(); 
    }
}


////////////////////////////////////////////////////////////
Worker::Worker() : m_Running(false)
{}


////////////////////////////////////////////////////////////
Worker::~Worker() 
{ 
    stop(); 
}


////////////////////////////////////////////////////////////
void Worker::start()
{
    {
        std::lock_guard<std::mutex> lk(m_Mutex);
        if (m_Running) 
        {
            return;
        }
        m_Running = true;
    }

    private_start();
}


////////////////////////////////////////////////////////////
void Worker::stop()
{
    {
        std::lock_guard<std::mutex> lk(m_Mutex);
        if (not m_Running ) 
        {
            return;
        }
        m_Running = false;
    }

    m_Condition.notify_all();
    m_Thread.join();
}


////////////////////////////////////////////////////////////
void Worker::private_start()
{
    m_Thread = std::thread([this]
    {
        LOG_DEBUG << LOG_BRIGHT_BLUE << "Start worker_thread" << LOG_WHITE;
        for (;;)
        {
            decltype(m_Queue) local_queue;
            {
                std::unique_lock<std::mutex> lk(m_Mutex);
                m_Condition.wait(lk, [&] { 
                    // LOG_DEBUG << LOG_BRIGHT_BLUE << "Ready for notify" << LOG_WHITE;
                    return not m_Queue.empty() + not m_Running; 
                });

                if (not m_Running)
                {
                    for (auto& func : m_Queue)
                    {
                        func();
                    }

                    m_Queue.clear();
                    return;
                }

                std::swap(m_Queue, local_queue);
            }

            for (auto& func : local_queue)
            {
                func();
            }
        }
        // LOG_DEBUG << LOG_BRIGHT_BLUE << "worker_thread end working" << LOG_WHITE;
    });
}
} // namespace utils