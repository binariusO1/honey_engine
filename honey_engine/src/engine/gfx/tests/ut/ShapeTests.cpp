#include <gtest/gtest.h>
#include <memory>
#include "gfx/draw/Shape.hpp"
#include "gfx/geometry/figures/Rectangle.hpp"

#include "logger/Logger.hpp"

using namespace ::testing;
namespace
{
} // namespace
namespace  he::gfx::draw
{
class ShapeTests : public testing::Test
{
public:
    ShapeTests() = default;
    ~ShapeTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::draw::Shape>("shape", rectangle);
    }

    std::unique_ptr<he::gfx::draw::Shape> sut;
    std::shared_ptr<he::gfx::geometry::figures::Rectangle> rectangle = 
        std::make_shared<he::gfx::geometry::figures::Rectangle>(he::gfx::geometry::Size2Df(100.0, 100.0));
};

TEST_F(ShapeTests, whenCreate_shouldNotUpdateVertexArray)
{
    createSut();
    ASSERT_EQ(sut->getVertexArray().empty(), true);
}

TEST_F(ShapeTests, whenSetColor_thenGetSameValue)
{
    createSut();
    sut->setColor(gfx::Color::Green);
    ASSERT_EQ(sut->getColor(), gfx::Color::Green);
}

TEST_F(ShapeTests, whenSetRotation_thenGetSameValue)
{
    createSut();
    he::gfx::geometry::Angle angle(46);
    sut->setRotation(angle);
    ASSERT_EQ(sut->getRotation(), angle);
}

TEST_F(ShapeTests, whenCheckIsPointInside_thenReturnTrue)
{
    createSut();
    const he::gfx::geometry::Point2Df pointToCheck{99.0, 100.0};
    ASSERT_EQ(sut->isPointInside(pointToCheck), true);
}

TEST_F(ShapeTests, whenCheckIsPointInside_thenReturnFalse)
{
    createSut();
    const he::gfx::geometry::Point2Df pointToCheck{101.0, 101.0};
    ASSERT_EQ(sut->isPointInside(pointToCheck), false);
}

TEST_F(ShapeTests, whenSetOriginAndNoSetPosition_thenCheckIsPointInside_thenReturnFalse)
{
    createSut();
    const he::gfx::geometry::Point2Df newOrigin{100.0, 100.0};
    const he::gfx::geometry::Point2Df pointToCheck{99.0, 99.0};
    // note: after setOrigin(100, 100), figure still stick to (0,0), so coordinates will be ({-100,-100}, {0, -100} ...)
    sut->setOrigin(newOrigin);

    ASSERT_EQ(sut->isPointInside(pointToCheck), false);
}

TEST_F(ShapeTests, whenSetOriginAndSetPosition_thenCheckIsPointInside_thenReturnFalse)
{
    createSut();
    const he::gfx::geometry::Point2Df newOrigin{100.0, 100.0};
    const he::gfx::geometry::Point2Df pointToCheck{101.0, 101.0};
    sut->setPosition(newOrigin);
    sut->setOrigin(newOrigin);

    ASSERT_EQ(sut->isPointInside(pointToCheck), false);
}

TEST_F(ShapeTests, whenSetOriginInCenterAndNoSetPosition_thenCheckIsPointInside_thenReturnTrue)
{
    createSut();
    const he::gfx::geometry::Point2Df pointToCheck{49.0, 49.0};
    sut->setOriginInCenter();

    ASSERT_EQ(sut->isPointInside(pointToCheck), true);
}

TEST_F(ShapeTests, whenSetOriginInCenterAndNoSetPosition_thenCheckIsPointInside_thenReturnFalse)
{
    createSut();
    const he::gfx::geometry::Point2Df pointToCheck{51.0, 51.0};
    sut->setOriginInCenter();

    ASSERT_EQ(sut->isPointInside(pointToCheck), false);
}

TEST_F(ShapeTests, whenSetPosition_thenCheckIsPointInside_thenReturnTrue)
{
    createSut();
    const he::gfx::geometry::Point2Df position{100.0, 100.0};
    sut->setPosition(position);
    const he::gfx::geometry::Point2Df pointToCheck{101.0, 101.0};
    ASSERT_EQ(sut->isPointInside(pointToCheck), true);
}

TEST_F(ShapeTests, whenSetPosition_thenCheckIsPointInside_thenReturnFalse)
{
    createSut();
    const he::gfx::geometry::Point2Df position{100.0, 100.0};
    sut->setPosition(position);
    const he::gfx::geometry::Point2Df pointToCheck{99.0, 99.0};
    ASSERT_EQ(sut->isPointInside(pointToCheck), false);
}

} // namespace he::gfx::draw