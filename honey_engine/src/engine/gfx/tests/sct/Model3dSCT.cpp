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
    rectangleShape->setOriginInCenter();
    rectangleShape->setPosition({f_defaultWindowWidth/2.f, f_defaultWindowHeight/2.f, 200});
    rectangleShape->setRotations(45, 0, 0, geometry::transform::AxisOrder::XYZ);
    rectangleShape->setColor(gfx::Color::Red);
    main3dLayer->addShape(rectangleShape);

    geometry::transform::Transform projection;
    projection.perspective(45.f, f_defaultWindowWidth/f_defaultWindowHeight, 0.1f, 100.f);
    //main3dLayer->setProjectionMatrix(projection.getMatrix());

    LOG_DEBUG << "Origin: " << rectangleShape->getOrigin();
    LOG_DEBUG << rectangleShape->getPosition();

    std::shared_ptr<he::gfx::draw::Shape2d> rectangleShape2 = std::make_shared<he::gfx::draw::Shape2d>("rectangleShape2", rectangle);
    rectangleShape2->setPosition({static_cast<float>(f_defaultWindowWidth-200), 0.0});
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