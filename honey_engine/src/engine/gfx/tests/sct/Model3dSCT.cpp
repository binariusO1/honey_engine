#include <libs/glad/glad.h> // note: it must be higher than other gl libraries
#include "RenderFixture.hpp"
#include <gtest/gtest.h>
#include "math/Matrix.hpp"
#include "gfx/geometry/transform/Transform.hpp"
#include "gfx/draw/AdvButton.hpp"

using namespace ::testing;

namespace  he
{
namespace gfx
{
class Model3dSCT : public testing::Test , public RenderFixture
{
public:
    Model3dSCT()
    {
    }
    ~Model3dSCT() = default;

    void createCustomScreen()
    {
        RenderFixture::createCustomScreen();
        enableEventInputListener();
    }

    draw::AdvButton createCustomAdvButtonInWindowCenter1()
    {
        he::gfx::draw::AdvButton button1("Button1", {100, 100});
        button1.setColor(he::gfx::Color::Green);
        button1.setPosition({static_cast<float>(f_defaultWindowWidth-100), 0});
        button1.setOriginInCenter();
        return button1;
    }
    draw::AdvButton createCustomAdvButtonInWindowCenter2()
    {
        he::gfx::draw::AdvButton button1("Button2", {100, 100});
        button1.setColor(he::gfx::Color::Red);
        button1.setPosition({static_cast<float>(f_defaultWindowWidth-200), 0});
        button1.setOriginInCenter();
        return button1;
    }
};

TEST_F(Model3dSCT, screenTest_drawSampleShapeIn3dSpace)
{
    createCustomScreen();

    geometry::transform::Transform projection(1.f);
    projection.perspective(45.f, f_defaultWindowWidth/f_defaultWindowHeight, 0.1f, 100.f);
    main3dLayer->setProjectionMatrix(projection.getMatrix());

    he::gfx::geometry::figures::Rectangle rectangle({400.0, 1.0});
    std::shared_ptr<he::gfx::draw::Shape2dFor3d> rectangleShape1 = std::make_shared<he::gfx::draw::Shape2dFor3d>("rectangleShape1", rectangle);
    rectangleShape1->setOriginInCenter();
    rectangleShape1->setPosition({0, -500, -6});
    rectangleShape1->setRotations(90, 0, 0, geometry::transform::AxisOrder::XYZ);
    rectangleShape1->setColor(gfx::Color::Red);


    /*
    geometry::transform::Transform model(1.0f);
    model.rotateAroundX(geometry::Angle{-55.f});

    geometry::transform::Transform view(1.0f);
    view.translate({0.f, 0.f, -3.f});

    geometry::transform::Transform projection;
    projection.perspective(45.f, f_defaultWindowWidth/f_defaultWindowHeight, 0.1f, 100.f);

    // LOG_DEBUG << math::toString(projection.getMatrix());
    */
    LOG_DEBUG << "Origin rectangleShape1: " << rectangleShape1->getOrigin();
    LOG_DEBUG << "Position rectangleShape1: " << rectangleShape1->getPosition();

    main3dLayer->addShape(rectangleShape1);

    geometry::figures::Rectangle g({static_cast<float>(f_defaultWindowWidth), f_defaultWindowHeight/2.f});
    std::shared_ptr<he::gfx::draw::Shape2d> ground = std::make_shared<he::gfx::draw::Shape2d>("ground", g);
    ground->setPosition({0.f, f_defaultWindowHeight/2.f});
    ground->setColor(Color::Cyan);

    std::shared_ptr<he::gfx::draw::AdvButton> button1 = std::make_shared<he::gfx::draw::AdvButton>(createCustomAdvButtonInWindowCenter1());
     he::gfx::draw::ButtonCallback callbackClickedTouched1 = [&button1, &rectangleShape1](){
        rectangleShape1->setPosition({0, -400, rectangleShape1->getPosition().z+1.f});
    };
    std::shared_ptr<he::gfx::draw::AdvButton> button2 = std::make_shared<he::gfx::draw::AdvButton>(createCustomAdvButtonInWindowCenter2());
     he::gfx::draw::ButtonCallback callbackClickedTouched2 = [&button2, &rectangleShape1](){
        rectangleShape1->setPosition({0, -400, rectangleShape1->getPosition().z-1.f});
    };
    button1->setCallback(callbackClickedTouched1, {ButtonState::Touched, ButtonState::Clicked});
    button2->setCallback(callbackClickedTouched2, {ButtonState::Touched, ButtonState::Clicked});
    addButtonToMainLayer(button1);
    addButtonToMainLayer(button2);
       addShapeToMainLayer(ground);
    display(500);
}
}// namespace gfx
}// namespace he