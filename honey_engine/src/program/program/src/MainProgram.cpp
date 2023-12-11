#include "Program.hpp"

#include "window/window/Window.hpp"
#include <memory>
#include "gfx/geometry/figures/Rectangle.hpp"
#include "gfx/draw/Shape.hpp"
#include "gfx/render/Layer.hpp"
#include "gfx/render/Render.hpp"
#include "gfx/render/Scene.hpp"
#include "gfx/render/SceneManager.hpp"
#include "sys/honey_engine.h"
#include "logger/Logger.hpp"

namespace he
{
bool Program::isExists{false};

//////////////////////////////////////////////////////////////////////
Program::Program()
{
    if (Program::isExists)
    {
        LOG_ERROR << "Only one instance of Program can exist!";
        throw;
    }
    Program::isExists = true;
}


//////////////////////////////////////////////////////////////////////
int Program::run(char* argv[])
{
    LOG_INFO << LOG_GREEN << "Run program: " << argv[0] << LOG_WHITE;

    return startProgram();
}


//////////////////////////////////////////////////////////////////////
int Program::startProgram()
{
        std::unique_ptr<he::window::Window> window = std::make_unique<he::window::Window>(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, WINDOW_NAME);
        std::unique_ptr<he::gfx::render::IRender> render = std::make_unique<he::gfx::render::Render>();

        he::gfx::geometry::figures::Rectangle rect(100.0);
        std::shared_ptr<he::gfx::draw::Shape> rectangle1 = std::make_shared<he::gfx::draw::Shape>("rect1", rect);
        rectangle1->setPosition({512, 350});

        std::shared_ptr<he::gfx::render::Layer> layer1 = std::make_shared<he::gfx::render::Layer>("layer_1");
        layer1->addShape(rectangle1);

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

        return 0;
}
} // namespace he