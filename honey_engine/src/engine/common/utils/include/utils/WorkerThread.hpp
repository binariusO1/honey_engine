#include <condition_variable>
#include <functional>
#include <list>
#include <map>
#include <mutex>
#include <thread>

namespace utils
{
class Worker
{
public:
    Worker(bool start);
    Worker();
    ~Worker();

    template<typename... Args>
    void push_task(Args&&... args)
    {
        {
            std::lock_guard<std::mutex> lk(m_Mutex);
            m_Queue.push_back(std::bind(std::forward<Args>(args)...));
        }

        m_Condition.notify_all();
    }

    void start();
    void stop();

private:
    void private_start();

private:
    std::condition_variable m_Condition;
    std::list<std::function<void()>> m_Queue;
    std::mutex m_Mutex;
    std::thread m_Thread;
    bool m_Running = false;
};
} // namespace utils