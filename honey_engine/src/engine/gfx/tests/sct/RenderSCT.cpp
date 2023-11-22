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
class RenderSCT : public testing::Test
{
public:
    RenderSCT()
    {
        window = std::make_unique<he::window::Window>(defaultWindowWidth, defaultWindowHeight, defaultWindowName);
        render = std::make_unique<he::gfx::render::Render>();
    }
    ~RenderSCT() = default;

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

TEST_F(RenderSCT, screenTestShapes_drawSamples)
{
    const he::gfx::render::RenderSettings renderSettings1{       
        he::libs::gl::DrawType::Static,
        he::libs::gl::ConnectionType::TriangleFan,
        false};
    // initialize rectangles
    std::shared_ptr<he::gfx::geometry::figures::Figure> rect = std::make_shared<he::gfx::geometry::figures::Rectangle>(he::gfx::geometry::Size2Df{200.0, 100.0});
    he::gfx::draw::Shape shape1("rect1", rect);
    shape1.setColor(he::gfx::Color::Yellow);
    shape1.setPosition({0.0, 100.0});
    std::shared_ptr<he::gfx::draw::IDrawable> rectangle1 = std::make_shared<he::gfx::draw::Shape>(shape1);
    std::shared_ptr<he::gfx::draw::IDrawable> rectangle2 = std::make_shared<he::gfx::draw::Shape>("rect2", rect);
    he::gfx::draw::Shape shape3("rect3", rect);
    shape3.setColor(he::gfx::Color::Green);
    shape3.setPosition({0.0, 200.0});
    std::shared_ptr<he::gfx::draw::IDrawable> rectangle3 = std::make_shared<he::gfx::draw::Shape>(shape3);

    std::shared_ptr<he::gfx::draw::Shape> shape4 = std::make_shared<he::gfx::draw::Shape>("rect4", rect);
    shape4->setColor(he::gfx::Color::Cyan);
    shape4->setPosition({0.0, 300.0});
    std::shared_ptr<he::gfx::draw::IDrawable> rectangle4 = std::shared_ptr<he::gfx::draw::Shape>(shape4);

    he::gfx::draw::Shape shape5("rect5", rect);
    shape5.setColor(he::gfx::Color::Blue);
    shape5.setPosition({0.0, 400.0});
    std::shared_ptr<he::gfx::draw::IDrawable> rectangle5 = std::make_shared<he::gfx::draw::Shape>(shape5);
    he::gfx::draw::Shape shape6("rect6", rect);
    shape6.setColor(he::gfx::Color::Magenta);
    shape6.setPosition({0.0, 500.0});
    std::shared_ptr<he::gfx::draw::IDrawable> rectangle6 = std::make_shared<he::gfx::draw::Shape>(shape6);
    he::gfx::draw::Shape shape7("rect7", rect);
    shape7.setColor(he::gfx::Color::Red);
    shape7.setPosition({0.0, 600.0});
    std::shared_ptr<he::gfx::draw::IDrawable> rectangle7 = std::make_shared<he::gfx::draw::Shape>(shape7);

    std::shared_ptr<he::gfx::render::Layer> layer1 = std::make_shared<he::gfx::render::Layer>("layer_1");
    he::gfx::render::DrawableList rectangles{rectangle1, rectangle2, rectangle3, rectangle4, rectangle5, rectangle6, rectangle7};
    layer1->addDrawables(rectangles);

    // initialize hexagons
    std::shared_ptr<he::gfx::geometry::figures::Figure> hex = std::make_shared<he::gfx::geometry::figures::Hexagon>(57.0);
    he::gfx::draw::Shape hex1("hex1", hex);
    hex1.setPosition({200.0, 0.0});
    hex1.closeVertexArray();
    std::shared_ptr<he::gfx::draw::IDrawable> hexagon1 = std::make_shared<he::gfx::draw::Shape>(hex1);

    he::gfx::draw::Shape hex2("hex2", hex);
    hex2.setPosition({200.0, 100.0});
    hex2.setColor(he::gfx::Color::Yellow);
    hex2.closeVertexArray();
    std::shared_ptr<he::gfx::draw::IDrawable> hexagon2 = std::make_shared<he::gfx::draw::Shape>(hex2);

    he::gfx::draw::Shape hex3("hex3", hex);
    hex3.setPosition({200.0, 200.0});
    hex3.setColor(he::gfx::Color::Green);
    hex3.closeVertexArray();
    std::shared_ptr<he::gfx::draw::IDrawable> hexagon3 = std::make_shared<he::gfx::draw::Shape>(hex3);

    he::gfx::draw::Shape hex4("hex4", hex);
    std::shared_ptr<he::gfx::draw::IDrawable> hexagon4{nullptr};
    if (dynamic_cast<he::gfx::draw::Shape*>(&hex4) != nullptr)
    {
        hex4.setPosition({200.0, 300.0});
        hex4.setColor(he::gfx::Color::Cyan);
        hex4.closeVertexArray();
        hexagon4 = std::make_shared<he::gfx::draw::Shape>(hex4);
    }

    he::gfx::draw::Shape hex5("hex5", hex, he::gfx::Color::Blue, {200.0, 400.0}, he::gfx::OriginPosition::leftDown);
    hex5.closeVertexArray();
    std::shared_ptr<he::gfx::draw::IDrawable> hexagon5{nullptr};
    if (dynamic_cast<he::gfx::draw::Shape*>(&hex4) != nullptr)
    {
        hexagon5 = std::make_shared<he::gfx::draw::Shape>(hex5);
    }

    he::gfx::draw::Shape hex6("hex6", hex);
    hex6.setColor(he::gfx::Color::Magenta);
    hex6.setPosition({200.0, 500.0});
    hex6.closeVertexArray();
    std::shared_ptr<he::gfx::draw::IDrawable> hexagon6 = std::make_shared<he::gfx::draw::Shape>(hex6);

    he::gfx::draw::Shape hex7("hex7", hex);
    hex7.setColor(he::gfx::Color::Red);
    hex7.setPosition({200.0, 600.0});
    hex7.closeVertexArray();
    std::shared_ptr<he::gfx::draw::IDrawable> hexagon7 = std::make_shared<he::gfx::draw::Shape>(hex7);

    std::shared_ptr<he::gfx::render::Layer> layer2 = std::make_shared<he::gfx::render::Layer>("layer_2");
    he::gfx::render::DrawableList hexagons{hexagon1, hexagon2, hexagon3, hexagon4, hexagon5, hexagon6, hexagon7};
    layer2->addDrawables(hexagons);

    const he::gfx::render::RenderSettings renderSettings2{       
        he::libs::gl::DrawType::Static,
        he::libs::gl::ConnectionType::LineStrip,
        false};

    // initialize cross
    std::shared_ptr<he::gfx::geometry::figures::Figure> cross = std::make_shared<he::gfx::geometry::figures::Cross>(50.0);
    std::shared_ptr<he::gfx::draw::Shape> cross1 = std::make_shared<he::gfx::draw::Shape>("cross1", cross);
    cross1->setPosition({320.0, 0.0});

    he::gfx::PropagationSettings propagationSettings{2, 7, 105.0, 100.0};
    std::shared_ptr<he::gfx::render::CopyPropagationLayer> layer3 = std::make_shared<he::gfx::render::CopyPropagationLayer>("layer_3", propagationSettings);
    layer3->addDrawable(cross1);

    //initialize texture drawable
    const he::gfx::geometry::Size2Dpxl textureSize(200, 100);
    std::shared_ptr<he::gfx::render::Texture> texture1 = std::make_shared<he::gfx::render::Texture>();
    texture1->createEmpty(textureSize);
    std::shared_ptr<he::gfx::draw::Shape> sprite1 = std::make_shared<he::gfx::draw::Sprite>("sprite1", texture1);
    sprite1->setPosition({540.0, 0.0});
    std::shared_ptr<he::gfx::render::Texture> texture2 = std::make_shared<he::gfx::render::Texture>("data\\test\\container.png");
    std::shared_ptr<he::gfx::draw::Shape> sprite2 = std::make_shared<he::gfx::draw::Sprite>("sprite2", texture2);
    sprite2->setPosition({540.0, 100.0});

    std::shared_ptr<he::gfx::render::Layer> layer4 = std::make_shared<he::gfx::render::Layer>("layer_4");
    he::gfx::render::DrawableList sprites{sprite1, sprite2};
    layer4->addDrawables(sprites);

    // scenes
    std::shared_ptr<he::gfx::render::Scene> scene1 = std::make_shared<he::gfx::render::Scene>("scene_1");
    // layer1->setRenderSettings(renderSettings1);
    layer2->setRenderSettings(renderSettings2);
    layer3->setRenderSettings(renderSettings2);
    scene1->addLayer(layer1);
    scene1->addLayer(layer2);
    scene1->addLayer(layer3);
    scene1->addLayer(layer4);
    he::gfx::render::SceneTransitionTable transitionTable{{scene1, nullptr, nullptr, nullptr, nullptr}};
    sceneManager = std::make_unique<he::gfx::render::SceneManager>(transitionTable);

    display(timeToDisplay);
}

TEST_F(RenderSCT, screenTestShapes_drawTextureBackground)
{
    std::shared_ptr<he::gfx::render::Texture> texture1 = std::make_shared<he::gfx::render::Texture>("data\\gfx\\interface\\menu_background.png");
    std::shared_ptr<he::gfx::draw::IDrawable> sprite1 = std::make_shared<he::gfx::draw::Sprite>("sprite1", texture1);

    std::shared_ptr<he::gfx::render::Layer> layer1 = std::make_shared<he::gfx::render::Layer>("layer_1");
    he::gfx::render::DrawableList sprites{sprite1};
    layer1->addDrawables(sprites);

    std::shared_ptr<he::gfx::render::Scene> scene1 = std::make_shared<he::gfx::render::Scene>("scene_1");
    scene1->addLayer(layer1);
    he::gfx::render::SceneTransitionTable transitionTable{{scene1, nullptr, nullptr, nullptr, nullptr}};
    sceneManager = std::make_unique<he::gfx::render::SceneManager>(transitionTable);

    display(timeToDisplay);
} 

TEST_F(RenderSCT, screenTestText_drawText)
{
    he::gfx::draw::Text text1("text2");
    text1.setString("Test text");
    std::shared_ptr<he::gfx::draw::IDrawable> text2 = std::make_shared<he::gfx::draw::Text>(text1);
    
    std::shared_ptr<he::gfx::render::Layer> layer1 = std::make_shared<he::gfx::render::Layer>("layer_1");
    he::gfx::render::DrawableList sprites{text2};
    layer1->addDrawables(sprites);

    std::shared_ptr<he::gfx::render::Scene> scene1 = std::make_shared<he::gfx::render::Scene>("scene_1");
    scene1->addLayer(layer1);
    he::gfx::render::SceneTransitionTable transitionTable{{scene1, nullptr, nullptr, nullptr, nullptr}};
    sceneManager = std::make_unique<he::gfx::render::SceneManager>(transitionTable);    

    display(timeToDisplay);
}
}// namespace he