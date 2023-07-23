#pragma once

#include <chrono>
#include <memory>
#include "gl_wrapper/Source.hpp"
#include "geometry/Point2d.hpp"
#include "sfml2/window/Event.hpp"
#include "utils/Chronometer.hpp"

namespace gl_wrapper
{
class Window
{
public:
    Window();

    int createWindow(int, int, const char*);
    void terminate();

    void clear();
    GLFWwindow* getWindow() { return window; }
    geometry::Point2Dd getCursorPos() const;
    bool isWindowOpen();
    void closeWindow();

    void pollEvent();
    bool processInput(sfml2::Event&);
    void swapBuffers();

public:
    void guardFrame();
    void setFramerateLimit(unsigned int limit);
    void startFrameGuard();
    void endFrameGuard();

public:
    GLFWwindow* window;
    static std::unique_ptr<sfml2::Event> currentEvent;

private:
    bool isMouseMoved();
    geometry::Point2Dd oldPos{};
    std::chrono::microseconds m_frameTimeLimit{0};
    utils::Chronometer m_Chronometer;
    std::uint16_t m_frames{0};

    const std::chrono::microseconds SECOND{1000000};
};
} // namespace gl_wrapper