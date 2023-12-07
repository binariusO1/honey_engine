#pragma once

#include <libs/glad/glad.h> // note: it must be higher than other gl libraries
#include <GLFW/glfw3.h>
#include <functional>
#include <memory>
#include <list>
#include "window/events/Event.hpp"
#include "window/events/IEventInputListener.hpp"
#include "window/events/IEventInputSender.hpp"
#include "window/events/WindowState.hpp"
#include "libs/glfw/GlfwWrapper.hpp"

namespace he
{
namespace window
{

using Point2Dd = std::pair<double, double>;
using Point2D = std::pair<int, int>;
using Size2d = std::pair<int, int>;
using NotifyEvent = std::function<void(const he::window::Event)>;

class Window : public IEventInputSender
{
private:
    struct Size
    {
        int width;
        int height;
    };

public:
    Window(int, int, const char*, WindowState = he::window::WindowState::windowed);
    ~Window();

public:
    void clear();
    bool isWindowOpen() const;
    void swapBuffers();
    void terminate();
    void enableEvents();
    void disableEvents();

    GLFWwindow* getWindow() const;
    Size2d const getWindowSize() const;
    WindowState getWindowState() const;

    // note: WindowProcessEvent.cpp
    void update();
    void update(he::window::Event&);
    const he::window::Event& getEvent() const;

    void addEventInputListener(IEventInputListener*) override;
    void removeEventInputListener(IEventInputListener*) override;

private:
    int createWindow(int, int, const char*);

    void closeWindow();
    void resize(int, int, WindowState = he::window::WindowState::windowed);

    // note: WindowProcessEvent.cpp
    bool process_event(he::window::Event&);
    void fillEvent(he::window::Event&);
    bool isMouseMoved();

    void handleKeyEvent(int, int, int, int);
    void handleMouseEvent(int, int);
    void handleScrollEvent(double, double);
    void handleMouseMoveEvent(double, double);
    void notifyListeners(const he::window::Event&) override;

    GLFWwindow* m_window;
    WindowState m_windowState;
    Size m_windowSize;
    Point2Dd m_oldPos{0.0, 0.0};
    he::window::Event m_currentEvent{};
    he::window::Point2D m_currentMousePosition{0, 0};
    NotifyEvent m_notifyEvent{nullptr};
    std::list<IEventInputListener*> m_eventInputListeners{};

    bool m_isWindowClosed{false};

protected:
    std::shared_ptr<he::window::IGlfwWrapper> m_glfwWrapper;
};
} // namespace he
} // namespace window