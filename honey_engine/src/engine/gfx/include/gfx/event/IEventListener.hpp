#pragma once

#include <utility>
#include "window/events/Event.hpp"

// TODO: DO USUNIĘCIA LUB ZAMIANY
namespace he
{
class IEventListener
{
public:
    virtual ~IEventListener() = default;

public:
    virtual void process_event(const he::window::Event&) = 0;
};
} // namespace he