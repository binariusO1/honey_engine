
#include "logger/Logger.hpp"
#include "gl_wrapper/window/Window.hpp"

#define TEST 0

int main()
{
    LOG_INFO << "\nStart system";

    auto width = 1024;
    auto height = 768;

    gl_wrapper::Window window;
    window.createWindow(width, height, "honey engine");
    window.terminate();
}

