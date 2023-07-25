#pragma once

#include <utility>
#include "Event.hpp"

namespace he
{
namespace window
{
class IEventInputListener;

class IEventInputSender
{
public:
    virtual ~IEventInputSender() = default;

public:
    virtual void addEventInputListener(IEventInputListener*) = 0;
    virtual void removeEventInputListener(IEventInputListener*) = 0;
    virtual void notifyListeners(const he::window::Event&) = 0;
};
} // namespace window
} // namespace he