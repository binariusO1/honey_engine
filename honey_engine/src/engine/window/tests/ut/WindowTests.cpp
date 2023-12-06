#include "window/window/Window.hpp"
#include <gmock/gmock-nice-strict.h>
#include <gtest/gtest.h>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include "events/EventInputListenerMock.hpp"
#include "exception/render_error.hpp"
#include "glfw/GlfwMock.hpp"

using namespace ::testing;

namespace
{
const char* defaultWindowName{"window test"};
constexpr int defaultWindowWidth{1024};
constexpr int defaultWindowHeight{768};
} // namespace
namespace  he::window
{
class TestedWindow : public Window
{
public:
    TestedWindow(
        int width, 
        int height, 
        const char* name, 
        std::shared_ptr<he::window::IGlfwWrapper> ptr, 
        he::window::WindowState state = he::window::WindowState::windowed) 
    : Window(width, height, name, state)
    {
        m_glfwWrapper = ptr;
    }
};

class WindowTestsFixture
{
public:
    void createSut()
    {
        sut = std::make_unique<he::window::TestedWindow>(defaultWindowWidth, defaultWindowHeight, defaultWindowName, mock);
        t_realWrapper = std::make_shared<he::window::GlfwWrapper>(sut->getWindow());
    }

    std::unique_ptr<he::window::TestedWindow> sut;
    std::shared_ptr<he::window::GlfwMock> mock = std::make_shared<he::window::GlfwMock>();
    std::shared_ptr<he::window::GlfwWrapper> t_realWrapper;
};

class WindowTests : public testing::Test , public WindowTestsFixture
{
public:
    WindowTests() = default;
    ~WindowTests() override {};
};

class WindowCreateFailTests : public testing::TestWithParam<std::tuple<int, int, const char*>>
{
};

TEST_P(WindowCreateFailTests, whenCreateWindow_shouldThrowException)
{
    const auto [width, height, defaultWindowName] = GetParam();
    EXPECT_THROW(Window(width, height, defaultWindowName), he::common::render_error);
}

INSTANTIATE_TEST_SUITE_P(
    WindowCreateFailInstantiate, 
    WindowCreateFailTests,
    testing::Values(
        std::make_tuple(0, 1, defaultWindowName),
        std::make_tuple(1, 0, defaultWindowName),
        std::make_tuple(std::numeric_limits<int>::min(), std::numeric_limits<int>::min(), defaultWindowName)));

TEST_F(WindowTests, whenCreateWindowWithMaximizedState_shouldMaximizeWindow)
{
    TestedWindow window(6, 5, "maximized window", mock, he::window::WindowState::maximized);
    auto size = window.getWindowSize();
    ASSERT_GT(size.first, 6);
    ASSERT_GT(size.second, 5);
}

TEST_F(WindowTests, shouldTerminateWindow)
{
    createSut();
    sut->terminate();
    ASSERT_EQ(sut->isWindowOpen(), true);
}

TEST_F(WindowTests, whenGetWindow_shouldReturnInitializedInstance)
{
    createSut();
    auto newWindow = sut->getWindow();
    ASSERT_NE(newWindow, nullptr);
}

TEST_F(WindowTests, whenPollEventCatchClosedEventType_shouldProcessAndCloseWindow)
{
    EXPECT_CALL(*mock, setMouseButtonCallback(_)).Times(0);
    EXPECT_CALL(*mock, pollEvents()).Times(1);

    createSut();

    he::window::Event event{};
    auto expectedEventType = he::window::Event::closed;
    event.type = expectedEventType;

    sut->update(event);

    ASSERT_EQ(event.type, expectedEventType);
    ASSERT_EQ(sut->isWindowOpen(), false);
}

class WindowResizedEventTests : public testing::TestWithParam<std::tuple<int, int, he::window::WindowState, const char*>> , public WindowTestsFixture
{
public:
    void callExpected(const char* name, int width, int height)
    {
        if (name == "not_change")
        {
            EXPECT_CALL(*mock, setWindowSize(width, height)).Times(0);
        }
        else if (name == "resize")
        {
            EXPECT_CALL(*mock, setWindowSize(width, height)).Times(1);
        }
        else if (name == "resize_maximized")
        {
            EXPECT_CALL(*mock, setWindowStateToMaximized()).Times(1);
        }
        else if (name == "not_resize_minimized")
        {
            EXPECT_CALL(*mock, setWindowStateToMinimized()).Times(1);
        }
        else if (name == "resize_windowedFullScreen")
        {
            EXPECT_CALL(*mock, setWindowStateToWindowedFullScreen()).Times(1);
        }
        EXPECT_CALL(*mock, getWindowSize());
    }

    void realCallExpected(const char* name, int width, int height)
    {
        if (name == "resize")
        {
            t_realWrapper->setWindowSize(width, height);//sut->getWindow()
        }
        else if (name == "resize_maximized")
        {
            t_realWrapper->setWindowStateToMaximized();
        }
        else if (name == "not_resize_minimized")
        {
            t_realWrapper->setWindowStateToMinimized();
        }
        else if (name == "resize_windowedFullScreen")
        {
            t_realWrapper->setWindowStateToWindowedFullScreen();
        }
    }

    void assertExpected(const char* name, int width, int height)
    {
        auto realWidth = t_realWrapper->getWindowSize().first; //sut->getWindow()
        auto realHeight = t_realWrapper->getWindowSize().second;

        if (name == "not_change")
        {
            ASSERT_EQ(realWidth, defaultWindowWidth);
            ASSERT_EQ(realHeight, defaultWindowHeight);
        }
        else if (name == "resize_maximized" or name == "resize_windowedFullScreen")
        {
            ASSERT_NE(realWidth, 0);
            ASSERT_NE(realHeight, 0);
        }
        else
        {
            ASSERT_EQ(realWidth, width);
            ASSERT_EQ(realHeight, height);
        }
    }
};

TEST_P(WindowResizedEventTests, whenPollEventCatchResizedEventType_shouldProcess)
{
    const auto [width, height, state, expectetResult] = GetParam();

    EXPECT_CALL(*mock, pollEvents()).Times(1);
    callExpected(expectetResult, width, height);

    he::window::Event event{};
    event.type = he::window::Event::resized;
    event.size.width = width;
    event.size.height = height;
    event.size.state = state;

    createSut();

    sut->update(event);
    realCallExpected(expectetResult, width, height);

    ASSERT_EQ(sut->getEvent().type, he::window::Event::resized);
    ASSERT_EQ(sut->getWindowState(), state);
    assertExpected(expectetResult, width, height);
}

INSTANTIATE_TEST_SUITE_P(
    WindowTests, 
    WindowResizedEventTests,
    testing::Values(
        std::make_tuple(0, 0, he::window::WindowState::windowed, "not_change"),
        std::make_tuple(defaultWindowWidth, defaultWindowHeight, he::window::WindowState::windowed, "not_change"),
        std::make_tuple(-1, -1, he::window::WindowState::windowed, "not_change"),
        std::make_tuple(1200, 800, he::window::WindowState::windowed, "resize"),
        std::make_tuple(0, 0, he::window::WindowState::maximized, "resize_maximized"),
        std::make_tuple(0, 0, he::window::WindowState::minimized, "not_resize_minimized"),
        std::make_tuple(0, 0, he::window::WindowState::windowedFullScreen, "resize_windowedFullScreen")));

TEST_F(WindowTests, whenPollEventCatchResizedEventTypeWithNoSize_shouldProcessButDoNothing)
{
    EXPECT_CALL(*mock, setMouseButtonCallback(_)).Times(0);
    EXPECT_CALL(*mock, setWindowSize(_, _)).Times(0);
    EXPECT_CALL(*mock, getWindowSize()).Times(1);
    EXPECT_CALL(*mock, pollEvents()).Times(1);

    createSut();

    he::window::Event event{};
    event.type = he::window::Event::resized;
    event.text.unicode = 0;

    sut->update(event);

    ASSERT_EQ(sut->getWindowState(), he::window::WindowState::windowed);
    ASSERT_EQ(sut->getEvent().type, he::window::Event::resized);
    ASSERT_EQ(sut->isWindowOpen(), true);
}

TEST_F(WindowTests, whenPollEventlostFocusEventType_shouldProcessAndDoNothing)
{
    EXPECT_CALL(*mock, setMouseButtonCallback(_)).Times(0);
    EXPECT_CALL(*mock, pollEvents()).Times(1);

    createSut();

    he::window::Event event{};
    auto expectedEventType = he::window::Event::lostFocus;
    event.type = expectedEventType;

    sut->update(event);

    ASSERT_EQ(sut->isWindowOpen(), true);
}

TEST_F(WindowTests, whenPollEventgainedFocusEventType_shouldProcessAndDoNothing)
{
    EXPECT_CALL(*mock, setMouseButtonCallback(_)).Times(0);
    EXPECT_CALL(*mock, pollEvents()).Times(1);

    createSut();

    he::window::Event event{};
    auto expectedEventType = he::window::Event::gainedFocus;
    event.type = expectedEventType;

    sut->update(event);

    ASSERT_EQ(sut->isWindowOpen(), true);
}

TEST_F(WindowTests, whenPollEventtextEnteredEventType_shouldProcessAndDoNothing)
{
    EXPECT_CALL(*mock, setMouseButtonCallback(_)).Times(0);
    EXPECT_CALL(*mock, pollEvents()).Times(1);

    createSut();

    he::window::Event event{};
    auto expectedEventType = he::window::Event::textEntered;
    event.type = expectedEventType;

    sut->update(event);

    ASSERT_EQ(sut->isWindowOpen(), true);
}

class WindowKeyEventTests : public testing::TestWithParam<std::tuple<int, int, he::window::Event::EventType>> , public WindowTestsFixture
{
};

TEST_P(WindowKeyEventTests, whenPollEventCatchKeyEventTypeWithEnabledEvents_shouldProcessSingleEvent)
{
    const auto [key, action, expectedEventType] = GetParam();

    EXPECT_CALL(*mock, pollEvents()).Times(1);

    he::window::Event event{};

    createSut();

    EXPECT_CALL(*mock, getCursorPos()).Times(1);
    EXPECT_CALL(*mock, setKeyCallback(A<GLFWkeyfun>())).WillOnce(Invoke(
        [window = sut->getWindow(), key = key, action = action]
        (GLFWkeyfun callback) {
            callback(window, key, 0, action, 0);
        }));
    EXPECT_CALL(*mock, setMouseButtonCallback(A<GLFWmousebuttonfun>()));
    EXPECT_CALL(*mock, setScrollCallback(A<GLFWscrollfun>()));

    sut->enableEvents();
    sut->update(event);

    ASSERT_EQ(event.type, expectedEventType);
    ASSERT_EQ(event.key.code, key);
}

INSTANTIATE_TEST_SUITE_P(
    WindowTests, 
    WindowKeyEventTests,
    testing::Values(
        std::make_tuple(GLFW_KEY_SPACE, GLFW_PRESS, he::window::Event::keyPressed),
        std::make_tuple(GLFW_KEY_SPACE, GLFW_RELEASE, he::window::Event::keyReleased),
        std::make_tuple(GLFW_KEY_UNKNOWN, GLFW_PRESS, he::window::Event::keyPressed),
        std::make_tuple(GLFW_KEY_ESCAPE, GLFW_PRESS, he::window::Event::keyPressed)));

class WindowMouseButtonEventTests : public testing::TestWithParam<std::tuple<int, int, int, int, he::window::Event::EventType>> , public WindowTestsFixture
{
};

TEST_P(WindowMouseButtonEventTests, whenPollEventCatchMouseButtonEventTypeWithEnabledEvents_shouldProcessSingleEvent)
{
    const auto [x, y, button, action, expectedEventType] = GetParam();

    EXPECT_CALL(*mock, pollEvents()).Times(1);
    EXPECT_CALL(*mock, getCursorPos()).Times(2).WillRepeatedly(Return(std::pair<int, int>(x, y)));

    he::window::Event event{};

    createSut();

    EXPECT_CALL(*mock, setMouseButtonCallback(A<GLFWmousebuttonfun>())).WillOnce(Invoke(
        [window = sut->getWindow(), button = button, action = action]
        (GLFWmousebuttonfun callback) {
            callback(window, button, action, 0);
        }));
    EXPECT_CALL(*mock, setKeyCallback(A<GLFWkeyfun>()));
    EXPECT_CALL(*mock, setScrollCallback(A<GLFWscrollfun>()));

    sut->enableEvents();
    sut->update(event);

    ASSERT_EQ(event.type, expectedEventType);
    ASSERT_EQ(event.mouseButton.button, button);
    ASSERT_EQ(event.mouseButton.x, x);
    ASSERT_EQ(event.mouseButton.y, defaultWindowHeight-y);
}

INSTANTIATE_TEST_SUITE_P(
    WindowTests, 
    WindowMouseButtonEventTests,
    testing::Values(
        std::make_tuple(0, 0, GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, he::window::Event::mouseButtonPressed),
        std::make_tuple(150, 1, GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, he::window::Event::mouseButtonReleased),
        std::make_tuple(1, 10, GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS, he::window::Event::mouseButtonPressed),
        std::make_tuple(1588, 6544, GLFW_MOUSE_BUTTON_RIGHT, GLFW_RELEASE, he::window::Event::mouseButtonReleased)));

class WindowMouseScrollEventTests : public testing::TestWithParam<std::tuple<double, double, he::window::Event::EventType, he::window::Mouse::Wheel>> , public WindowTestsFixture
{
};

TEST_P(WindowMouseScrollEventTests, whenPollEventCatchMouseScrollEventTypeWithEnabledEvents_shouldProcessSingleEvent)
{
    const auto [xoffset, yoffset, expectedEventType, wheel] = GetParam();

    EXPECT_CALL(*mock, pollEvents()).Times(1);
    EXPECT_CALL(*mock, getCursorPos()).Times(1);

    he::window::Event event{};

    createSut();

    EXPECT_CALL(*mock, setScrollCallback(A<GLFWscrollfun>())).WillOnce(Invoke(
        [window = sut->getWindow(), xoffset = xoffset, yoffset = yoffset]
        (GLFWscrollfun callback) {
            callback(window, xoffset, yoffset);
        }));
    EXPECT_CALL(*mock, setMouseButtonCallback(A<GLFWmousebuttonfun>()));
    EXPECT_CALL(*mock, setKeyCallback(A<GLFWkeyfun>()));

    sut->enableEvents();
    sut->update(event);

    ASSERT_EQ(event.type, expectedEventType);
    ASSERT_EQ(event.mouseWheelScroll.wheel, wheel);
    ASSERT_EQ(event.mouseWheelScroll.x, xoffset);
    ASSERT_EQ(event.mouseWheelScroll.y, yoffset);
}

INSTANTIATE_TEST_SUITE_P(
    WindowTests, 
    WindowMouseScrollEventTests,
    testing::Values(
        std::make_tuple(-1.4, 0, he::window::Event::mouseWheelScrolled, he::window::Mouse::Wheel::HorizontalWheel),
        std::make_tuple(1.1, 0, he::window::Event::mouseWheelScrolled, he::window::Mouse::Wheel::HorizontalWheel),
        std::make_tuple(0, -2.5, he::window::Event::mouseWheelScrolled, he::window::Mouse::Wheel::VerticalWheel),
        std::make_tuple(0, 3.5, he::window::Event::mouseWheelScrolled, he::window::Mouse::Wheel::VerticalWheel),
        std::make_tuple(11.4, 3.5, he::window::Event::mouseWheelScrolled,he::window::Mouse::Wheel::VerticalWheel)));

TEST_F(WindowTests, whenPollEventDontCatchAnyEventType_shouldProcessSingleEventAsCursorPosition)
{
    int x = 123;
    int y = 777;
    EXPECT_CALL(*mock, pollEvents()).Times(1);
    EXPECT_CALL(*mock, getCursorPos()).Times(1).WillRepeatedly(Return(std::pair<int, int>(x, y)));

    createSut();

    he::window::Event event{};
    event.type = he::window::Event::none;

    EXPECT_CALL(*mock, setMouseButtonCallback(A<GLFWmousebuttonfun>()));
    EXPECT_CALL(*mock, setKeyCallback(A<GLFWkeyfun>()));
    EXPECT_CALL(*mock, setScrollCallback(A<GLFWscrollfun>()));

    sut->enableEvents();
    sut->update(event);

    ASSERT_EQ(event.type, he::window::Event::EventType::mouseCursorMoved);
    ASSERT_EQ(event.mouseMove.x, x);
    ASSERT_EQ(event.mouseMove.y, y);
}

TEST_F(WindowTests, whenCallbackInputEventCall_shouldNotifyAllListeners)
{
    std::unique_ptr<he::window::EventInputListenerMock> listener1 = std::make_unique<he::window::EventInputListenerMock>();
    std::unique_ptr<he::window::EventInputListenerMock> listener2 = std::make_unique<he::window::EventInputListenerMock>();

    he::window::Event event{};
    auto button = GLFW_MOUSE_BUTTON_LEFT;
    auto action = GLFW_PRESS;
    auto expectedEvent = he::window::Event::mouseButtonPressed;

    EXPECT_CALL(*mock, pollEvents()).Times(1);
    EXPECT_CALL(*mock, getCursorPos()).Times(2);
    EXPECT_CALL(*listener1, process_event(_)).Times(1);
    EXPECT_CALL(*listener2, process_event(_)).Times(1);

    createSut();

    sut->addEventInputListener(listener1.get());
    sut->addEventInputListener(listener2.get());

    EXPECT_CALL(*mock, setMouseButtonCallback(A<GLFWmousebuttonfun>())).WillOnce(Invoke(
        [window = sut->getWindow(), button = button, action = action]
        (GLFWmousebuttonfun callback) {
            callback(window, button, action, 0);
        }));
    EXPECT_CALL(*mock, setKeyCallback(A<GLFWkeyfun>()));
    EXPECT_CALL(*mock, setScrollCallback(A<GLFWscrollfun>()));

    sut->enableEvents();
    sut->update(event);

    ASSERT_EQ(event.type, he::window::Event::mouseButtonPressed);
    ASSERT_EQ(event.mouseButton.button, GLFW_MOUSE_BUTTON_LEFT);
}

TEST_F(WindowTests, whenCallbackInputEventCall_shouldNotifyRemainedListeners)
{
    std::unique_ptr<he::window::EventInputListenerMock> listener1 = std::make_unique<he::window::EventInputListenerMock>();
    std::unique_ptr<he::window::EventInputListenerMock> listener2 = std::make_unique<he::window::EventInputListenerMock>();
    std::unique_ptr<he::window::EventInputListenerMock> listener3 = nullptr;

    he::window::Event event{};
    auto button = GLFW_MOUSE_BUTTON_LEFT;
    auto action = GLFW_PRESS;
    auto expectedEvent = he::window::Event::mouseButtonPressed;

    EXPECT_CALL(*mock, pollEvents()).Times(1);
    EXPECT_CALL(*mock, getCursorPos()).Times(2);
    EXPECT_CALL(*listener2, process_event(_)).Times(1);

    createSut();

    sut->addEventInputListener(listener1.get());
    sut->addEventInputListener(listener2.get());
    sut->addEventInputListener(listener3.get());

    sut->removeEventInputListener(listener1.get());

    EXPECT_CALL(*mock, setMouseButtonCallback(A<GLFWmousebuttonfun>())).WillOnce(Invoke(
        [window = sut->getWindow(), button = button, action = action]
        (GLFWmousebuttonfun callback) {
            callback(window, button, action, 0);
        }));
    EXPECT_CALL(*mock, setKeyCallback(A<GLFWkeyfun>()));
    EXPECT_CALL(*mock, setScrollCallback(A<GLFWscrollfun>()));

    sut->enableEvents();
    sut->update(event);

    ASSERT_EQ(event.type, he::window::Event::mouseButtonPressed);
    ASSERT_EQ(event.mouseButton.button, GLFW_MOUSE_BUTTON_LEFT);
}
} // namespace he::window