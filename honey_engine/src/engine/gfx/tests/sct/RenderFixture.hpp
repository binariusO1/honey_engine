#pragma once

#include "gfx/render/Layer.hpp"
#include "gfx/render/Render.hpp"
#include "gfx/render/Scene.hpp"
#include "gfx/render/SceneManager.hpp"
#include "logger/Logger.hpp"
#include "window/window/Window.hpp"

namespace he
{
namespace gfx
{
class RenderFixture
{
public:
    RenderFixture()
    {
        mainWindow = std::make_unique<he::window::Window>(defaultWindowWidth, defaultWindowHeight, defaultWindowName);
        mainRender = std::make_unique<he::gfx::render::Render>();
    }

    void display(const size_t timeToDisplay = 600)
    {
        LOG_INFO << LOG_BRIGHT_BLUE << "[SCT] Start display" << LOG_WHITE;
        if (not mainSceneManager)
        {
            LOG_ERROR << "[SCT] mainSceneManager is not initialized. Please initialize it first";
            return;
        }

        auto i = timeToDisplay;
        while (mainWindow->isWindowOpen() and i > 0)
        {
            mainWindow->clear();
            mainSceneManager->render(*mainRender);
        
            mainWindow->swapBuffers();
            mainWindow->update();

            --i;
        }
        LOG_INFO << LOG_BRIGHT_BLUE << "[SCT] End display" << LOG_WHITE;
    }

    void createCustomScreen()
    {
        mainLayer = std::make_shared<he::gfx::render::Layer>("main_layer");
        mainScene = std::make_shared<he::gfx::render::Scene>("main_scene");
        mainScene->addLayer(mainLayer);
        he::gfx::render::SceneTransitionTable transitionTable{{mainScene, nullptr, nullptr, nullptr, nullptr}};
        mainSceneManager = std::make_unique<he::gfx::render::SceneManager>(transitionTable);
    }

    void addDrawableToMainLayer(const std::shared_ptr<he::gfx::draw::IDrawable>& drawable)
    {
        mainLayer->addDrawable(drawable);
    }

    void addButtonToMainLayer(const std::shared_ptr<he::gfx::draw::IButton>& drawable)
    {
        mainLayer->addButton(drawable);
    }

    void addDrawablesToMainLayer(const he::gfx::render::DrawableList& list)
    {
        mainLayer->addDrawables(list);
    }

    std::shared_ptr<he::gfx::render::Layer> mainLayer;
    std::shared_ptr<he::gfx::render::Scene> mainScene;
    std::shared_ptr<he::gfx::render::SceneManager> mainSceneManager{nullptr};
    std::unique_ptr<he::window::Window> mainWindow;
    std::unique_ptr<he::gfx::render::IRender> mainRender;

const char* t_fontCalibriPath = "\\data\\gfx\\fonts\\calibri.ttf";

    const char* defaultWindowName{"window test"};
    const int defaultWindowWidth{1200};
    const int defaultWindowHeight{800};
};
} // namespace gfx
} // namespace he