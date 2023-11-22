#include "window/window/Window.hpp"
#include <memory>
#include "gfx/geometry/figures/Rectangle.hpp"
#include "gfx/draw/Shape.hpp"
#include "gfx/render/Layer.hpp"
#include "gfx/render/Render.hpp"
#include "gfx/render/Scene.hpp"
#include "gfx/render/SceneManager.hpp"
#include "logger/Logger.hpp"
#include "sys/honey_engine.h"


#define TEST 0

int main()
{
    LOG_INFO << "\nStart system";

    std::unique_ptr<he::window::Window> window = std::make_unique<he::window::Window>(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, WINDOW_NAME);
    std::unique_ptr<he::gfx::render::IRender> render = std::make_unique<he::gfx::render::Render>();

    std::shared_ptr<he::gfx::geometry::figures::Figure> rect = std::make_shared<he::gfx::geometry::figures::Rectangle>(100.0);
    std::shared_ptr<he::gfx::draw::Shape> rectangle1 = std::make_shared<he::gfx::draw::Shape>("rect1", rect);
    rectangle1->setPosition({512, 350});

    std::shared_ptr<he::gfx::render::Layer> layer1 = std::make_shared<he::gfx::render::Layer>("layer_1");
    layer1->addDrawable(rectangle1);

    std::shared_ptr<he::gfx::render::Scene> scene1 = std::make_shared<he::gfx::render::Scene>("scene_1");
    scene1->addLayer(layer1);
    he::gfx::render::SceneTransitionTable transitionTable{{scene1, nullptr, nullptr, nullptr, nullptr}};
    std::unique_ptr<he::gfx::render::SceneManager> sceneManager = std::make_unique<he::gfx::render::SceneManager>(transitionTable);

    std::size_t i = 600;

    while(window->isWindowOpen() and i > 0)
    {
        window->clear();
    
        sceneManager->render(*render);
    
        window->swapBuffers();
        window->update();
        --i;
    }

    window->terminate();
}

