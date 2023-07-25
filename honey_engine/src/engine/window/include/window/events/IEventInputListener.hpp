#pragma once

#include <utility>
#include "Event.hpp"

namespace he
{
namespace window
{

class IEventInputListener
{
public:
    virtual ~IEventInputListener() = default;

public:
    virtual void process_event(const he::window::Event&) = 0;
};
} // namespace window
} // namespace he