#include <gtest/gtest.h>
#include "gfx/draw/Shape.hpp"
#include "gfx/geometry/figures/Rectangle.hpp"

using namespace ::testing;
namespace
{
} // namespace
namespace  he::gfx::draw
{
class Shape2dTests : public testing::Test
{
public:
    Shape2dTests() = default;
    ~Shape2dTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::draw::Shape2d>("shape2d", *rectangle);
    }

    std::unique_ptr<he::gfx::draw::Shape2d> sut;
    std::shared_ptr<he::gfx::geometry::figures::Rectangle> rectangle = 
        std::make_shared<he::gfx::geometry::figures::Rectangle>(he::gfx::geometry::Size2Df(100.0, 100.0));
};

TEST_F(Shape2dTests, whenCreate_shouldNotUpdateVertexArray)
{
    createSut();
    ASSERT_EQ(sut->getVertexArray().empty(), true);
}

TEST_F(Shape2dTests, whenSetColor_thenGetSameValue)
{
    createSut();
    sut->setColor(gfx::Color::Green);
    ASSERT_EQ(sut->getColor(), gfx::Color::Green);
}

TEST_F(Shape2dTests, whenSetRotation_thenGetSameValue)
{
    createSut();
    he::gfx::geometry::Angle angle(46);
    sut->setRotation(46);
    ASSERT_EQ(sut->getRotation(), angle);
}

TEST_F(Shape2dTests, whenCheckIsPointInside_thenReturnTrue)
{
    createSut();
    const he::gfx::geometry::Point2Df pointToCheck{99.0, 100.0};
    ASSERT_EQ(sut->isPointInside(pointToCheck), true);
}

TEST_F(Shape2dTests, whenCheckIsPointInside_thenReturnFalse)
{
    createSut();
    const he::gfx::geometry::Point2Df pointToCheck{101.0, 101.0};
    ASSERT_EQ(sut->isPointInside(pointToCheck), false);
}

TEST_F(Shape2dTests, whenSetOriginAndNoSetPosition_thenCheckIsPointInside_thenReturnFalse)
{
    createSut();
    const he::gfx::geometry::Point2Df newOrigin{100.0, 100.0};
    const he::gfx::geometry::Point2Df pointToCheck{99.0, 99.0};

    sut->setOrigin(newOrigin);

    ASSERT_EQ(sut->isPointInside(pointToCheck), false);
}

TEST_F(Shape2dTests, whenSetOriginAndSetPosition_thenCheckIsPointInside_thenReturnFalse)
{
    createSut();
    const he::gfx::geometry::Point2Df newOrigin{100.0, 100.0};
    const he::gfx::geometry::Point2Df pointToCheck{101.0, 101.0};
    sut->setPosition(newOrigin);
    sut->setOrigin(newOrigin);

    ASSERT_EQ(sut->isPointInside(pointToCheck), false);
}

TEST_F(Shape2dTests, whenSetOriginInCenterAndNoSetPosition_thenCheckIsPointInside_thenReturnTrue)
{
    createSut();
    const he::gfx::geometry::Point2Df pointToCheck{49.0, 49.0};
    sut->setOriginInCenter();

    ASSERT_EQ(sut->isPointInside(pointToCheck), true);
}

TEST_F(Shape2dTests, whenSetOriginInCenterAndNoSetPosition_thenCheckIsPointInside_thenReturnFalse)
{
    createSut();
    const he::gfx::geometry::Point2Df pointToCheck{51.0, 51.0};
    sut->setOriginInCenter();

    ASSERT_EQ(sut->isPointInside(pointToCheck), false);
}

TEST_F(Shape2dTests, isPointInside_thenCheckIsPointInside_thenReturnTrue)
{
    const he::gfx::geometry::Point2Df position{100.0, 100.0};
    const he::gfx::geometry::Point2Df pointToCheck{101.0, 101.0};

    createSut();
    sut->setPosition(position);

    ASSERT_EQ(sut->isPointInside(pointToCheck), true);
}

TEST_F(Shape2dTests, isPointInside_thenCheckIsPointInside_thenReturnFalse)
{
    const he::gfx::geometry::Point2Df position{100.0, 100.0};
    const he::gfx::geometry::Point2Df pointToCheck{99.0, 99.0};

    createSut();
    sut->setPosition(position);

    ASSERT_EQ(sut->isPointInside(pointToCheck), false);
}

TEST_F(Shape2dTests, setPosition_whenSetNewPosition_pointsShouldBeChanged)
{
    const he::gfx::geometry::Point2Df position{150.0, 140.0};

    createSut();
    auto vertexArray = sut->getVertexArray();
    sut->setPosition(position);

    for (size_t i = 0 ; i < vertexArray.size() ; ++i)
    {
        ASSERT_NE(sut->getVertexArray()[i].position, vertexArray[i].position);
    }
}

class Shape2dFor3dTestsWrapper : public Shape2dFor3d
{
public:
    ~Shape2dFor3dTestsWrapper() override {};

public:
    Shape2dFor3dTestsWrapper(const std::string& name, const geometry::figures::Figure& figure) : Shape2dFor3d(name, figure)
    {
    }

    void updateVertexArray() override
    {
        Shape2dFor3d::updateVertexArray();
    }
};

class Shape2dFor3dTests : public testing::Test
{
public:
    Shape2dFor3dTests() = default;
    ~Shape2dFor3dTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::draw::Shape2dFor3dTestsWrapper>("shape2dFor3d", *rectangle);
    }

    std::unique_ptr<he::gfx::draw::Shape2dFor3dTestsWrapper> sut;
    std::shared_ptr<he::gfx::geometry::figures::Rectangle> rectangle = 
        std::make_shared<he::gfx::geometry::figures::Rectangle>(he::gfx::geometry::Size2Df(100.0, 100.0));
};

TEST_F(Shape2dFor3dTests, setPosition_whenSetNewPosition_pointsShouldBeChanged)
{
    const he::gfx::geometry::Point3Df newPosition{150.f, 140.f, 250.f};
    const he::gfx::geometry::Point3Df expectedPositions[4]{
        {150.f, 140.f, 250.f}, 
        {250.f, 140.f, 250.f},
        {250.f, 240.f, 250.f},
        {150.f, 240.f, 250.f}
    };

    createSut();
    sut->setPosition(newPosition);
    sut->updateVertexArray();
    auto vertexArray = sut->getVertexArray();

    for (size_t i = 0 ; i < vertexArray.size() ; ++i)
    {
        he::math::transformPoint3d(vertexArray[i].position.x, vertexArray[i].position.y, vertexArray[i].position.z, sut->getTransform().getMatrix());
        ASSERT_EQ(vertexArray[i].position, expectedPositions[i]);
    }
}

TEST_F(Shape2dFor3dTests, setRotationsZ_whenSetNewPosition_pointsShouldBeChanged)
{
    const he::gfx::geometry::Point3Df newPosition{150.f, 140.f, 250.f};
    const he::gfx::geometry::Point3Df expectedPositions[4]{
        {150.f, 140.f, 250.f}, 
        {220.711, 69.2893, 250.f},
        {291.421, 140.f, 250.f},
        {220.711, 210.711, 250.f}
    };

    createSut();
    sut->setPosition(newPosition);
    sut->setRotations(0, 0, 45, geometry::transform::AxisOrder::XYZ);
    sut->updateVertexArray();
    auto vertexArray = sut->getVertexArray();

    for (size_t i = 0 ; i < vertexArray.size() ; ++i)
    {
        he::math::transformPoint3d(vertexArray[i].position.x, vertexArray[i].position.y, vertexArray[i].position.z, sut->getTransform().getMatrix());
        ASSERT_EQ(static_cast<int>(vertexArray[i].position.x), static_cast<int>(expectedPositions[i].x));
        ASSERT_EQ(static_cast<int>(vertexArray[i].position.y), static_cast<int>(expectedPositions[i].y));
        ASSERT_EQ(static_cast<int>(vertexArray[i].position.z), static_cast<int>(expectedPositions[i].z));
    }
}

TEST_F(Shape2dFor3dTests, setRotationsX_whenSetNewPosition_pointsShouldBeChanged)
{
    const he::gfx::geometry::Point3Df newPosition{150.f, 150.f, 250.f};
    const he::gfx::geometry::Point3Df expectedPositions[4]{
        {100.f, 114.645, 214.645}, 
        {200.f, 114.645, 214.645},
        {200.f, 185.355, 285.355},
        {100.f, 185.355, 285.355}
    };

    createSut();
    sut->setPosition(newPosition);
    sut->setOriginInCenter();
    sut->setRotations(45, 0, 0, geometry::transform::AxisOrder::XYZ);
    sut->updateVertexArray();
    auto vertexArray = sut->getVertexArray();

    for (size_t i = 0 ; i < vertexArray.size() ; ++i)
    {
        he::math::transformPoint3d(vertexArray[i].position.x, vertexArray[i].position.y, vertexArray[i].position.z, sut->getTransform().getMatrix());
        ASSERT_EQ(static_cast<int>(vertexArray[i].position.x), static_cast<int>(expectedPositions[i].x));
        ASSERT_EQ(static_cast<int>(vertexArray[i].position.y), static_cast<int>(expectedPositions[i].y));
        ASSERT_EQ(static_cast<int>(vertexArray[i].position.z), static_cast<int>(expectedPositions[i].z));
    }
}
} // namespace he::gfx::draw