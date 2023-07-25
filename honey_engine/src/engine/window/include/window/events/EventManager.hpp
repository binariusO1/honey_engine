#pragma once

#include <list>
#include "window/events/IEventInputListener.hpp"
#include "window/events/IEventInputSender.hpp"
#include "window/events/Event.hpp"

namespace he
{
namespace window       
{
class EventManager : public window::IEventInputListener , public window::IEventInputSender
{
public:
    EventManager();
    ~EventManager() override;

public:
    void process_event(const he::window::Event&);

public:
    void addEventInputListener(IEventInputListener*) override;
    void removeEventInputListener(IEventInputListener*) override;

private:
    void notifyListeners(const he::window::Event&) override;

private:
    std::list<IEventInputListener*> m_eventInputListeners{};
};
} // namespace window
} // namespace he