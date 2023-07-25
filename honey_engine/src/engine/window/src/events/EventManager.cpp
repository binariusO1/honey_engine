#include "window/events/EventManager.hpp"

#include "logger/Logger.hpp"

namespace he
{
namespace window
{
////////////////////////////////////////////////////////////
EventManager::EventManager()
{
}


////////////////////////////////////////////////////////////
EventManager::~EventManager()
{
}


////////////////////////////////////////////////////////////
void EventManager::process_event(const he::window::Event& event)
{
    notifyListeners(event);
}


////////////////////////////////////////////////////////////
void EventManager::addEventInputListener(IEventInputListener* listener)
{
    m_eventInputListeners.push_back(listener);
}



////////////////////////////////////////////////////////////
void EventManager::removeEventInputListener(IEventInputListener* listener)
{
    m_eventInputListeners.remove(listener);
}


////////////////////////////////////////////////////////////
void EventManager::notifyListeners(const he::window::Event& event)
{
    auto it = m_eventInputListeners.begin();
    while (it != m_eventInputListeners.end())
    {
        IEventInputListener* element = *it;
        if (element != nullptr) 
        {
            element->process_event(event);
            ++it;
        } 
        else 
        {
            it = m_eventInputListeners.erase(it);
        }
    }
}
} // namespace window
} // namespace he