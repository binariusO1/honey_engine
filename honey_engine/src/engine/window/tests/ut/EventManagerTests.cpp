#include "window/events/EventManager.hpp"
#include <gtest/gtest.h>
#include <libs/glad/glad.h> // note: it must be higher than other gl libraries
#include <GLFW/glfw3.h>
#include "events/EventInputListenerMock.hpp"

using namespace ::testing;


namespace
{
} // namespace

namespace  he::window
{
class EventManagerTests : public testing::Test
{
public:
    EventManagerTests() = default;
    ~EventManagerTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::window::EventManager>();
    }

    std::unique_ptr<he::window::EventManager> sut;
};

TEST_F(EventManagerTests, whenWindowEventIncome_shouldNotifyAllListeners)
{
    std::unique_ptr<he::window::EventInputListenerMock> listener1 = std::make_unique<he::window::EventInputListenerMock>();
    std::unique_ptr<he::window::EventInputListenerMock> listener2 = std::make_unique<he::window::EventInputListenerMock>();

    he::window::Event event{};
    auto button = GLFW_MOUSE_BUTTON_LEFT;
    auto action = GLFW_PRESS;
    auto expectedEvent = he::window::Event::mouseButtonPressed;

    EXPECT_CALL(*listener1, process_event(_)).Times(1);
    EXPECT_CALL(*listener2, process_event(_)).Times(1);

    createSut();

    sut->addEventInputListener(listener1.get());
    sut->addEventInputListener(listener2.get());

    sut->process_event(event);
}

TEST_F(EventManagerTests, whenWindowEventIncome_shouldNotifyRemainedListeners)
{
    std::unique_ptr<he::window::EventInputListenerMock> listener1 = std::make_unique<he::window::EventInputListenerMock>();
    std::unique_ptr<he::window::EventInputListenerMock> listener2 = std::make_unique<he::window::EventInputListenerMock>();
    std::unique_ptr<he::window::EventInputListenerMock> listener3 = nullptr;

    he::window::Event event{};
    auto button = GLFW_MOUSE_BUTTON_LEFT;
    auto action = GLFW_PRESS;
    auto expectedEvent = he::window::Event::mouseButtonPressed;

    EXPECT_CALL(*listener2, process_event(_)).Times(1);

    createSut();

    sut->addEventInputListener(listener1.get());
    sut->addEventInputListener(listener2.get());
    sut->removeEventInputListener(listener1.get());
    sut->process_event(event);
}
} // he::window