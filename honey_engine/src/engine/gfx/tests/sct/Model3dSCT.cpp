#include <libs/glad/glad.h> // note: it must be higher than other gl libraries
#include "RenderFixture.hpp"
#include <gtest/gtest.h>
#include "math/Matrix.hpp"
#include "gfx/geometry/transform/Transform.hpp"
//#include "logger/Logger.hpp"

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

TEST_F(Model3dSCT, screenTest_drawSampleModel)
{
    createCustomScreen();

    geometry::transform::Transform model(1.0f);
    model.rotateAroundX(geometry::Angle{-55.f});

    geometry::transform::Transform view(1.0f);
    view.translate({0.f, 0.f, -3.f});

    geometry::transform::Transform projection;
    projection.perspective(45.f, f_defaultWindowWidth/f_defaultWindowHeight, 0.1f, 100.f);

    LOG_DEBUG << math::toString(projection.getMatrix());

    display(500);
}
}// namespace gfx
}// namespace he