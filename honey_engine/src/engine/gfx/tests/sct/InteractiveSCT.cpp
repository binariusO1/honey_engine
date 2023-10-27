#include "window/window/Window.hpp"
#include <gtest/gtest.h>
#include <memory>
#include "gfx/geometry/Size2d.hpp"
#include "gfx/geometry/figures/Cross.hpp"
#include "gfx/geometry/figures/Hexagon.hpp"
#include "gfx/geometry/figures/Rectangle.hpp"
#include "gfx/draw/Shape.hpp"
#include "gfx/draw/Sprite.hpp"
#include "gfx/draw/Text.hpp"
#include "gfx/graphic/Texture.hpp"
#include "gfx/render/Layer.hpp"
#include "gfx/render/CopyPropagationLayer.hpp"
#include "gfx/render/PropagationSettings.hpp"
#include "gfx/render/Render.hpp"
#include "gfx/render/RenderSettings.hpp"
#include "gfx/render/Scene.hpp"
#include "gfx/render/SceneManager.hpp"
#include "logger/Logger.hpp"

using namespace ::testing;
namespace
{
const char* defaultWindowName{"window test"};
constexpr int defaultWindowWidth{1200};
constexpr int defaultWindowHeight{800};
constexpr std::size_t timeToDisplay{150};
} // namespace
namespace  he
{
class InteractiveSCT : public testing::Test
{
public:
    InteractiveSCT()
    {
        window = std::make_unique<he::window::Window>(defaultWindowWidth, defaultWindowHeight, defaultWindowName);
        render = std::make_unique<he::gfx::render::Render>();
    }
    ~InteractiveSCT() = default;

    void display(const size_t timeToDisplay)
    {
        if (not sceneManager)
        {
            LOG_WARNING << "sceneManager is not initialized. Please initialize it first";
            return;
        }

        auto i = timeToDisplay;
        while (window->isWindowOpen() and i > 0)
        {
            window->clear();
            sceneManager->render(*render);
        
            window->swapBuffers();
            window->update();

            --i;
        }
    }

    std::unique_ptr<he::window::Window> window;
    std::unique_ptr<he::gfx::render::IRender> render;
    std::unique_ptr<he::gfx::render::SceneManager> sceneManager{nullptr};
};

TEST_F(InteractiveSCT, buttonTest)
{
}

TEST_F(InteractiveSCT, eventTest)
{
}

TEST_F(InteractiveSCT, menuTest)
{
}

}// namespace he