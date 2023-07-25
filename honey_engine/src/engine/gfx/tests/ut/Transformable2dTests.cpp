
#include <gtest/gtest.h>
#include "gfx/geometry/transform/Transformable2d.hpp"
#include "logger/Logger.hpp"
namespace  he::gfx::geometry::transform
{
class Transformable2dTests : public testing::Test
{
public:
    Transformable2dTests() = default;
    ~Transformable2dTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::geometry::transform::Transformable2d>();
    }
    std::unique_ptr<he::gfx::geometry::transform::Transformable2d> sut;

};

TEST_F(Transformable2dTests, whenMoveThenGetNewPosition)
{
    createSut();
    sut->move({100.0, 100.0});
    ASSERT_EQ(sut->getPosition(), he::gfx::geometry::Point2Df(100.0, 100.0));
}

TEST_F(Transformable2dTests, whenSetOriginThenGetSameValue)
{
    createSut();
    sut->setOrigin({110.0, 110.0});
    ASSERT_EQ(sut->getOrigin(), he::gfx::geometry::Point2Df(110.0, 110.0));
}

TEST_F(Transformable2dTests, whenSetRotationThenGetSameValue)
{
    createSut();
    he::gfx::geometry::Angle angle(135.0);
    sut->setRotation(angle);
    ASSERT_EQ(sut->getRotation(), angle);
}

TEST_F(Transformable2dTests, whenSetScaleThenGetSameValue)
{
    createSut();
    sut->setScale({10.0, 1.1});
    ASSERT_EQ(sut->getScale(), he::gfx::geometry::Vector2Df(10.0, 1.1));
}

TEST_F(Transformable2dTests, whenSetPositionThenGetSameValue)
{
    createSut();
    sut->setPosition({115.0, 455.1});
    ASSERT_EQ(sut->getPosition(), he::gfx::geometry::Point2Df(115.0, 455.1));
}

TEST_F(Transformable2dTests, checkPointPosition_whenSetPosition)
{
    createSut();
    Point2Df point1({0, 0});
    Point2Df point2({200, 0});
    Point2Df point3({0, 200});
    Point2Df point4({200, 200});

    sut->setPosition({150.0, 150.0});
    sut->transformPoint(point1);
    sut->transformPoint(point2);
    sut->transformPoint(point3);
    sut->transformPoint(point4);

    ASSERT_EQ(point1, he::gfx::geometry::Point2Df(150.0, 150.0));
    ASSERT_EQ(point2, he::gfx::geometry::Point2Df(350.0, 150.0));
    ASSERT_EQ(point3, he::gfx::geometry::Point2Df(150.0, 350.0));
    ASSERT_EQ(point4, he::gfx::geometry::Point2Df(350.0, 350.0));
}

TEST_F(Transformable2dTests, checkPointPosition_whenSetScale)
{
    createSut();
    Point2Df point1({0, 0});
    Point2Df point2({200, 0});
    Point2Df point3({0, 200});
    Point2Df point4({200, 200});
    sut->setOrigin({100.0, 100.0});
    sut->setScale({2.0, 0.5});
    sut->transformPoint(point1);
    sut->transformPoint(point2);
    sut->transformPoint(point3);
    sut->transformPoint(point4);

    ASSERT_EQ(point1, he::gfx::geometry::Point2Df(-200.0, -50.0));
    ASSERT_EQ(point2, he::gfx::geometry::Point2Df(200.0, -50.0));
    ASSERT_EQ(point3, he::gfx::geometry::Point2Df(-200.0, 50.0));
    ASSERT_EQ(point4, he::gfx::geometry::Point2Df(200.0, 50.0));
}

TEST_F(Transformable2dTests, checkInverseTransformPoint)
{
    Transformable2d transformableObject;
    Point2Df point({100, 100});
    const Point2Df pointBeforeTransform = point;

    transformableObject.setOrigin({75, 75});
    transformableObject.setPosition({200, 200});
    transformableObject.setScale({0.5f, 0.5f});
    transformableObject.setRotation(Angle(45.0f));
    transformableObject.transformPoint(point);

    ASSERT_NE(static_cast<int>(point.x), static_cast<int>(pointBeforeTransform.x));
    ASSERT_NE(static_cast<int>(point.y), static_cast<int>(pointBeforeTransform.y));

    transformableObject.inverseTransformPoint(point);
    ASSERT_EQ(static_cast<int>(point.x), static_cast<int>(pointBeforeTransform.x));
    ASSERT_EQ(static_cast<int>(point.y), static_cast<int>(pointBeforeTransform.y));
}
} // namespace he::gfx::geometry::transform