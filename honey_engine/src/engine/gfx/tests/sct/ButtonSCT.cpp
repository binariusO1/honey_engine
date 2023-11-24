#include "window/window/Window.hpp"
#include <gtest/gtest.h>
#include <memory>
#include "gfx/geometry/Size2d.hpp"
#include "gfx/geometry/figures/Cross.hpp"
#include "gfx/geometry/figures/Hexagon.hpp"
#include "gfx/geometry/figures/Rectangle.hpp"
#include "gfx/draw/Button.hpp"
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
class ButtonSCT : public testing::Test
{
public:
    ButtonSCT()
    {
        window = std::make_unique<he::window::Window>(defaultWindowWidth, defaultWindowHeight, defaultWindowName);
        render = std::make_unique<he::gfx::render::Render>();
    }
    ~ButtonSCT() = default;

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
    std::shared_ptr<he::gfx::render::SceneManager> sceneManager{nullptr};
};

TEST_F(ButtonSCT, buttonTest)
{
}

TEST_F(ButtonSCT, eventTest_afterMoveButtonWithDefaultTextToWindowCenter_shouldClick)
{
    const he::gfx::geometry::Size2Dpxl textureSize(200, 100);
    std::shared_ptr<he::gfx::render::Texture> texture1 = std::make_shared<he::gfx::render::Texture>();
    texture1->createEmpty(textureSize);
    he::gfx::draw::Button button1("Button1", texture1);
    button1.setColor(he::gfx::Color::Blue);
    button1.setText("Quit");
    button1.setPosition({574, 350});
    std::shared_ptr<he::gfx::draw::Button> but1 = std::make_shared<he::gfx::draw::Button>(button1);

    std::shared_ptr<he::gfx::render::Layer> layer1 = std::make_shared<he::gfx::render::Layer>("layer_1");
    he::gfx::render::DrawableList sprites{but1};
    layer1->addDrawables(sprites);
    layer1->addButton(but1);

    std::shared_ptr<he::gfx::render::Scene> scene1 = std::make_shared<he::gfx::render::Scene>("scene_1");
    scene1->addLayer(layer1);
    he::gfx::render::SceneTransitionTable transitionTable{{scene1, nullptr, nullptr, nullptr, nullptr}};
    sceneManager = std::make_shared<he::gfx::render::SceneManager>(transitionTable);    

    std::shared_ptr<he::window::IEventInputListener> sceneListener = std::shared_ptr<he::gfx::render::SceneManager>(sceneManager);
    window->addEventInputListener(sceneListener.get());

    display(60*10);
}

TEST_F(ButtonSCT, menuTest)
{
}

}// namespace he