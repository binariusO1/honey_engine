#pragma once

#include <gmock/gmock-function-mocker.h>
#include "window/events/IEventInputListener.hpp"

namespace he
{
namespace window
{
class EventInputListenerMock : public IEventInputListener
{
public:
    EventInputListenerMock() = default;
    ~EventInputListenerMock() override = default;
    MOCK_METHOD(void, process_event, (const he::window::Event&), (override));
};
} // namespace window
} // namespace he