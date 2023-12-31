#include "RenderFixture.hpp"
#include <gtest/gtest.h>
#include "gfx/geometry/figures/Cross.hpp"
#include "gfx/geometry/figures/Hexagon.hpp"
#include "gfx/geometry/figures/Rectangle.hpp"
#include "gfx/draw/Shape.hpp"
#include "gfx/draw/Sprite.hpp"
#include "gfx/draw/Text.hpp"
#include "gfx/graphic/Texture.hpp"
#include "gfx/render/CopyPropagationLayer.hpp"
#include "gfx/render/PropagationSettings.hpp"

using namespace ::testing;
namespace
{
} // namespace


namespace  he
{
namespace gfx
{
class ShapeSCT : public testing::Test , public RenderFixture
{
public:
    ShapeSCT()
    {
    }
    ~ShapeSCT() = default;
};

TEST_F(ShapeSCT, screenTestShapes_drawSamples)
{
    createCustomScreen();
    const he::gfx::render::RenderSettings renderSettings1{       
        he::libs::gl::DrawType::Static,
        he::libs::gl::ConnectionType::TriangleFan,
        false};

    // initialize rectangles
    std::shared_ptr<he::gfx::geometry::figures::Figure> rect = std::make_shared<he::gfx::geometry::figures::Rectangle>(he::gfx::geometry::Size2Df{200.0, 100.0});
    he::gfx::draw::ShapeRectangle shape1("rect1", rect->getSize());
    shape1.setColor(he::gfx::Color::Yellow);
    shape1.setPosition({0.0, 100.0});
    std::shared_ptr<he::gfx::draw::IShape2d> rectangle1 = std::make_shared<he::gfx::draw::ShapeRectangle>(shape1);
    std::shared_ptr<he::gfx::draw::IShape2d> rectangle2 = std::make_shared<he::gfx::draw::ShapeRectangle>("rect2", rect->getSize());
    he::gfx::draw::ShapeRectangle shape3("rect3", rect->getSize());
    shape3.setColor(he::gfx::Color::Green);
    shape3.setPosition({0.0, 200.0});
    std::shared_ptr<he::gfx::draw::IShape2d> rectangle3 = std::make_shared<he::gfx::draw::ShapeRectangle>(shape3);

    std::shared_ptr<he::gfx::draw::ShapeRectangle> shape4 = std::make_shared<he::gfx::draw::ShapeRectangle>("rect4", rect->getSize());
    shape4->setColor(he::gfx::Color::Cyan);
    shape4->setPosition({0.0, 300.0});
    std::shared_ptr<he::gfx::draw::IShape2d> rectangle4 = std::shared_ptr<he::gfx::draw::ShapeRectangle>(shape4);

    he::gfx::draw::ShapeRectangle shape5("rect5", rect->getSize());
    shape5.setColor(he::gfx::Color::Blue);
    shape5.setPosition({0.0, 400.0});
    std::shared_ptr<he::gfx::draw::IShape2d> rectangle5 = std::make_shared<he::gfx::draw::ShapeRectangle>(shape5);
    he::gfx::draw::ShapeRectangle shape6("rect6", rect->getSize());
    shape6.setColor(he::gfx::Color::Magenta);
    shape6.setPosition({0.0, 500.0});
    std::shared_ptr<he::gfx::draw::IShape2d> rectangle6 = std::make_shared<he::gfx::draw::ShapeRectangle>(shape6);
    he::gfx::draw::ShapeRectangle shape7("rect7", rect->getSize());
    shape7.setColor(he::gfx::Color::Red);
    shape7.setPosition({0.0, 600.0});
    std::shared_ptr<he::gfx::draw::IShape2d> rectangle7 = std::make_shared<he::gfx::draw::ShapeRectangle>(shape7);

    std::shared_ptr<he::gfx::render::Layer2d> layer1 = std::make_shared<he::gfx::render::Layer2d>("layer_1");
    he::gfx::render::ShapeList rectangles{rectangle1, rectangle2, rectangle3, rectangle4, rectangle5, rectangle6, rectangle7};
    layer1->addShapes(rectangles);

    // initialize hexagons
    std::shared_ptr<he::gfx::geometry::figures::Figure> hex = std::make_shared<he::gfx::geometry::figures::Hexagon>(57.0);
    he::gfx::draw::Shape2d hex1("hex1", *hex);
    hex1.setPosition({200.0, 0.0});
    hex1.closeVertexArray();
    std::shared_ptr<he::gfx::draw::IShape2d> hexagon1 = std::make_shared<he::gfx::draw::Shape2d>(hex1);

    he::gfx::draw::Shape2d hex2("hex2", *hex);
    hex2.setPosition({200.0, 100.0});
    hex2.setColor(he::gfx::Color::Yellow);
    hex2.closeVertexArray();
    std::shared_ptr<he::gfx::draw::IShape2d> hexagon2 = std::make_shared<he::gfx::draw::Shape2d>(hex2);

    he::gfx::draw::Shape2d hex3("hex3", *hex);
    hex3.setPosition({200.0, 200.0});
    hex3.setColor(he::gfx::Color::Green);
    hex3.closeVertexArray();
    std::shared_ptr<he::gfx::draw::IShape2d> hexagon3 = std::make_shared<he::gfx::draw::Shape2d>(hex3);

    he::gfx::draw::Shape2d hex4("hex4", *hex);
    std::shared_ptr<he::gfx::draw::IShape2d> hexagon4{nullptr};

    if (dynamic_cast<he::gfx::draw::Shape2d*>(&hex4) != nullptr)
    {
        hex4.setPosition({200.0, 300.0});
        hex4.setColor(he::gfx::Color::Cyan);
        hex4.closeVertexArray();
        hexagon4 = std::make_shared<he::gfx::draw::Shape2d>(hex4);
    }

    he::gfx::draw::Shape2d hex5("hex5", *hex);
    hex5.setColor(he::gfx::Color::Blue);
    hex5.setPosition({200.0, 400.0});
    hex5.setOriginPosition(he::gfx::OriginPosition::leftDown);
    hex5.closeVertexArray();
    std::shared_ptr<he::gfx::draw::IShape2d> hexagon5{nullptr};

    if (dynamic_cast<he::gfx::draw::Shape2d*>(&hex4) != nullptr)
    {
        hexagon5 = std::make_shared<he::gfx::draw::Shape2d>(hex5);
    }

    he::gfx::draw::Shape2d hex6("hex6", *hex);
    hex6.setColor(he::gfx::Color::Magenta);
    hex6.setPosition({200.0, 500.0});
    hex6.closeVertexArray();
    std::shared_ptr<he::gfx::draw::IShape2d> hexagon6 = std::make_shared<he::gfx::draw::Shape2d>(hex6);

    he::gfx::draw::Shape2d hex7("hex7", *hex);
    hex7.setColor(he::gfx::Color::Red);
    hex7.setPosition({200.0, 600.0});
    hex7.closeVertexArray();
    std::shared_ptr<he::gfx::draw::IShape2d> hexagon7 = std::make_shared<he::gfx::draw::Shape2d>(hex7);

    std::shared_ptr<he::gfx::render::Layer2d> layer2 = std::make_shared<he::gfx::render::Layer2d>("layer_2");
    he::gfx::render::ShapeList hexagons{hexagon1, hexagon2, hexagon3, hexagon4, hexagon5, hexagon6, hexagon7};
    layer2->addShapes(hexagons);

    const he::gfx::render::RenderSettings renderSettings2{       
        he::libs::gl::DrawType::Static,
        he::libs::gl::ConnectionType::LineStrip,
        false};

    // initialize cross
    std::shared_ptr<he::gfx::geometry::figures::Figure> cross = std::make_shared<he::gfx::geometry::figures::Cross>(50.0);
    std::shared_ptr<he::gfx::draw::Shape2d> cross1 = std::make_shared<he::gfx::draw::Shape2d>("cross1", *cross);
    cross1->setPosition({320.0, 0.0});

    he::gfx::PropagationSettings propagationSettings{2, 7, 105.0, 100.0};
    std::shared_ptr<he::gfx::render::CopyPropagationLayer> layer3 = std::make_shared<he::gfx::render::CopyPropagationLayer>("layer_3", propagationSettings);
    layer3->addShape(cross1);

    //initialize texture drawable
    const he::gfx::geometry::Size2Dpxl textureSize(200, 100);
    std::shared_ptr<he::gfx::render::Texture> texture1 = std::make_shared<he::gfx::render::Texture>();
    texture1->createEmpty(textureSize);
    std::shared_ptr<he::gfx::draw::Shape2d> sprite1 = std::make_shared<he::gfx::draw::Sprite>("sprite1", texture1);
    sprite1->setPosition({540.0, 0.0});
    std::shared_ptr<he::gfx::render::Texture> texture2 = std::make_shared<he::gfx::render::Texture>("data\\test\\container.png");
    std::shared_ptr<he::gfx::draw::Shape2d> sprite2 = std::make_shared<he::gfx::draw::Sprite>("sprite2", texture2);
    sprite2->setPosition({540.0, 100.0});

    std::shared_ptr<he::gfx::render::Layer2d> layer4 = std::make_shared<he::gfx::render::Layer2d>("layer_4");
    he::gfx::render::ShapeList sprites{sprite1, sprite2};
    layer4->addShapes(sprites);

    // scenes
    std::shared_ptr<he::gfx::render::Scene> scene1 = std::make_shared<he::gfx::render::Scene>("scene_1");
    layer2->setRenderSettings(renderSettings2);
    layer3->setRenderSettings(renderSettings2);
    scene1->addLayer(layer1);
    scene1->addLayer(layer2);
    scene1->addLayer(layer3);
    scene1->addLayer(layer4);
    he::gfx::render::SceneTransitionTable transitionTable{{scene1, nullptr, nullptr, nullptr, nullptr}};
    mainSceneManager = std::make_unique<he::gfx::render::SceneManager>(transitionTable);

    display(200);
}

TEST_F(ShapeSCT, screenTestShapes_drawTextureBackground)
{
    // todo move to SpriteSCT
    createCustomScreen();
    std::shared_ptr<he::gfx::render::Texture> texture1 = std::make_shared<he::gfx::render::Texture>("data\\gfx\\interface\\menu_background.png");
    std::shared_ptr<he::gfx::draw::IShape2d> sprite1 = std::make_shared<he::gfx::draw::Sprite>("sprite1", texture1);

    addShapeToMainLayer(sprite1);

    display(200);
} 
}// namespace gfx
}// namespace he