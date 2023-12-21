#include <libs/glad/glad.h> // note: it must be higher than other gl libraries
#include "RenderFixture.hpp"
#include <gtest/gtest.h>
#include "math/Matrix.hpp"
#include "gfx/geometry/transform/Transform.hpp"

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
};

TEST_F(Model3dSCT, screenTest_drawSampleShapeIn3dSpace)
{
    createCustomScreen();

    he::gfx::geometry::figures::Rectangle rectangle({200.0, 200.0});
    std::shared_ptr<he::gfx::draw::Shape2dFor3d> rectangleShape = std::make_shared<he::gfx::draw::Shape2dFor3d>("rectangleShape", rectangle);
    rectangleShape->setRotation(45);
    rectangleShape->setColor(gfx::Color::Red);
    main3dLayer->addShape(rectangleShape);

    std::shared_ptr<he::gfx::draw::Shape2d> rectangleShape2 = std::make_shared<he::gfx::draw::Shape2d>("rectangleShape2", rectangle);
    rectangleShape2->setPosition({400.0, 0.0});
    LOG_DEBUG << rectangleShape->getOrigin();
    rectangleShape2->setRotations(0, 0, 0, geometry::transform::AxisOrder::XYZ);
    LOG_DEBUG << rectangleShape2->getOrigin();
    LOG_DEBUG << rectangleShape2->getPosition();
    main2dLayer->addShape(rectangleShape2);

    /*
    geometry::transform::Transform model(1.0f);
    model.rotateAroundX(geometry::Angle{-55.f});

    geometry::transform::Transform view(1.0f);
    view.translate({0.f, 0.f, -3.f});

    geometry::transform::Transform projection;
    projection.perspective(45.f, f_defaultWindowWidth/f_defaultWindowHeight, 0.1f, 100.f);

    // LOG_DEBUG << math::toString(projection.getMatrix());
    */



    display(500);
}
}// namespace gfx
}// namespace he