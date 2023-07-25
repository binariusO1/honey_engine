#pragma once

#include <chrono>
#include <memory>
#include "geometry/Point2d.hpp"
#include "Source.hpp"
#include "utils/Chronometer.hpp"
#include "window/Event.hpp"

namespace he
{
namespace window
{
class Window
{
public:
    Window(int, int, const char*);
    ~Window();

    void clear();
    void closeWindow();
    bool isWindowOpen();
    void swapBuffers();
    void terminate();

    GLFWwindow* getWindow();
    geometry::Point2Dd getCursorPos() const;

    // note: WindowProcessInput.cpp
    void pollEvent();
    bool processInput(he::window::Event&);

public:
    void guardFrame();
    void setFramerateLimit(unsigned int limit);
    void startFrameGuard();
    void endFrameGuard();

public:


private:
    int createWindow(int, int, const char*);
    // ->
    bool isMouseMoved();

    GLFWwindow* window;
    std::unique_ptr<he::window::Event> currentEvent;
    bool isWindowClosed{false};
    // ->
    geometry::Point2Dd oldPos{};
    std::chrono::microseconds m_frameTimeLimit{0};
    utils::Chronometer m_Chronometer;
    std::uint16_t m_frames{0};

    const std::chrono::microseconds SECOND{1000000};
};
} // namespace he
} // namespace window