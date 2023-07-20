
#include <gtest/gtest.h>
#include "geometry/figures/Cross.hpp"
#include "geometry/figures/Hexagon.hpp"
#include "geometry/figures/Polygon.hpp"
#include "geometry/figures/Rectangle.hpp"

namespace  geometry::figures
{
////////////////////////////////////////////////////////////
// Cross
////////////////////////////////////////////////////////////
class CrossTest : public testing::Test
{
};

TEST_F(CrossTest, whenGetNumOfPoints_shouldReturnCorrectValue)
{
    Cross cross(0.0);
    ASSERT_EQ(cross.getNumOfPoints(), 5);
}

TEST_F(CrossTest, whenGetCenterPoint_shouldReturnCorrectValue)
{
    Cross cross(155.55);
    const geometry::Point2Df excpectedCenterPoint{155.55, 155.55};

    ASSERT_EQ(cross.getCenterPoint(), excpectedCenterPoint);
}

class CrossGetPointsTest : public testing::TestWithParam<std::pair<std::size_t, geometry::Point2Df>>
{
};

TEST_P(CrossGetPointsTest, whenGetPointIndex_shouldReturnPointValue)
{
    Cross cross(1.0);
    const auto [index, excpectValue] = GetParam();

    ASSERT_EQ(cross.getPoint(index), excpectValue);
}

INSTANTIATE_TEST_SUITE_P(
    CrossGetPointsInstantiate, 
    CrossGetPointsTest,
    testing::Values(
        std::make_pair(0, geometry::Point2Df{0.0, 1.0}),
        std::make_pair(1, geometry::Point2Df{2.0, 1.0}),
        std::make_pair(2, geometry::Point2Df{1.0, 1.0}),
        std::make_pair(3, geometry::Point2Df{1.0, 2.0}),
        std::make_pair(4, geometry::Point2Df{1.0, 0.0})));

TEST_F(CrossTest, whenGetPointAndValueOutOfRange_shouldThrowException)
{
    const float arm = 200.0;
    Cross cross(arm);

    ASSERT_THROW(cross.getPoint(10), std::out_of_range);
}

TEST_F(CrossTest, whenGetSize_shouldReturnMaxCountur)
{
    const float arm = 100.0;
    const geometry::Size2Df expectedSize{200.0, 200.0};
    Cross cross(arm);

    ASSERT_EQ(cross.getSize(), expectedSize);
}

class CrossCheckPointInsideTest : public testing::TestWithParam<std::pair<geometry::Point2Df, bool>>
{
};

TEST_P(CrossCheckPointInsideTest, whenCheckIsPointInside_shouldReturnStatement)
{
    Cross cross(2.0);
    const auto [value, expectedStatement] = GetParam();

    ASSERT_EQ(cross.isPointInside(value), expectedStatement);
}

INSTANTIATE_TEST_SUITE_P(
    CrossCheckPointInsideInstantiate, 
    CrossCheckPointInsideTest,
    testing::Values(
        std::make_pair(geometry::Point2Df{0.0, 2.0}, true),
        std::make_pair(geometry::Point2Df{1.0, 2.0}, true),
        std::make_pair(geometry::Point2Df{2.0, 2.0}, true),
        std::make_pair(geometry::Point2Df{0.0, 0.0}, false),
        std::make_pair(geometry::Point2Df{3.0, 1.0}, false),
        std::make_pair(geometry::Point2Df{4.0, 4.0}, false)));

TEST_F(CrossTest, shouldReturnCorrectArmLength_AfterSetSize)
{
    const float arm = 199.9;
    Cross cross(arm);
    cross.setSize(177.66);
    const float expectArmLenght{177.66};

    ASSERT_EQ(cross.getArmLenght(), expectArmLenght);
}


////////////////////////////////////////////////////////////
// Hexagon
////////////////////////////////////////////////////////////
class HexagonTest : public testing::Test
{
};

TEST_F(HexagonTest, whenGetNumOfPoints_shouldReturnCorrectValue)
{
    Hexagon hexagon(0.0);

    ASSERT_EQ(hexagon.getNumOfPoints(), 6);
}

TEST_F(HexagonTest, whenGetCenterPoint_shouldReturnCorrectValue)
{
    Hexagon hexagon(155.55);
    const geometry::Point2Df excpectedCenterPoint{155.55, 134.71};

    ASSERT_EQ(hexagon.getCenterPoint().x, excpectedCenterPoint.x);
    ASSERT_EQ(static_cast<int>(hexagon.getCenterPoint().y * 1000), static_cast<int>(excpectedCenterPoint.y *1000));
}

class HexagonGetPointsTest : public testing::TestWithParam<std::pair<std::size_t, geometry::Point2Df>>
{
};

TEST_P(HexagonGetPointsTest, whenGetPointIndex_shouldReturnPointValue)
{
    Hexagon hexagon(3.0);
    const auto [index, excpectValue] = GetParam();

    ASSERT_EQ(static_cast<int>(hexagon.getPoint(index).x * 1000), static_cast<int>(excpectValue.x * 1000));
    ASSERT_EQ(static_cast<int>(hexagon.getPoint(index).y * 1000), static_cast<int>(excpectValue.y * 1000));
}

INSTANTIATE_TEST_SUITE_P(
    HexagonGetPointsInstantiate, 
    HexagonGetPointsTest,
    testing::Values(
        std::make_pair(0, geometry::Point2Df{0.0, 2.598}),
        std::make_pair(1, geometry::Point2Df{1.5, 0.0}),
        std::make_pair(2, geometry::Point2Df{4.5, 0.0}),
        std::make_pair(3, geometry::Point2Df{6.0, 2.598}),
        std::make_pair(4, geometry::Point2Df{4.5, 5.196}),
        std::make_pair(5, geometry::Point2Df{1.5, 5.196})));

TEST_F(HexagonTest, whenGetPointAndValueOutOfRange_shouldThrowException)
{
    Hexagon hexagon(300);
    ASSERT_THROW(hexagon.getPoint(10), std::out_of_range);
}

TEST_F(HexagonTest, whenGetSize_shouldReturnMaxCountur)
{
    const float side = 100.0;
    const geometry::Size2Df expectedSize{200.0, 173.205};
    Hexagon hexagon(side);

    ASSERT_EQ(static_cast<int>(hexagon.getSize().width * 1000), static_cast<int>(expectedSize.width * 1000));
    ASSERT_EQ(static_cast<int>(hexagon.getSize().height * 1000), static_cast<int>(expectedSize.height * 1000));
}

class HexagonCheckPointInsideTest : public testing::TestWithParam<std::pair<geometry::Point2Df, bool>>
{
};

TEST_P(HexagonCheckPointInsideTest, whenCheckIsPointInside_shouldReturnStatement)
{
    Hexagon hexagon(150.0);
    const auto [value, expectedStatement] = GetParam();

    ASSERT_EQ(hexagon.isPointInside(value), expectedStatement);
}

INSTANTIATE_TEST_SUITE_P(
    HexagonCheckPointInsideInstantiate, 
    HexagonCheckPointInsideTest,
    testing::Values(
        std::make_pair(geometry::Point2Df{150.0, 150.0}, true),
        std::make_pair(geometry::Point2Df{75.0, 250.0}, true),
        std::make_pair(geometry::Point2Df{69.0, 250.0}, false),
        std::make_pair(geometry::Point2Df{250.0, 235.0}, false),
        std::make_pair(geometry::Point2Df{299.999, 129.903}, true),
        std::make_pair(geometry::Point2Df{129.904, 129.904}, true)));

TEST_F(HexagonTest, whenCheckPointOnTheBorder_shouldReturnTrue)
{
    Hexagon hexagon(200);
    geometry::Point2Df point1{300, 0};
    geometry::Point2Df point2{100, 0};
    ASSERT_EQ(true, hexagon.isPointInside(point1));
    ASSERT_EQ(true, hexagon.isPointInside(point2));
}

TEST_F(HexagonTest, shouldReturnCorrectSideLenght_AfterSetSide)
{
    const float side = 199.9;
    Hexagon hexagon(side);
    hexagon.setSide(177.66);
    const float expectSideLenght{177.66};

    ASSERT_EQ(hexagon.getSideLenght(), expectSideLenght);
}


////////////////////////////////////////////////////////////
// Polygon
////////////////////////////////////////////////////////////
class PolygonTest : public testing::Test
{
};

TEST_F(PolygonTest, whenGetPointAndValueOutOfRange_shouldThrowException)
{
    Polygon polygon;
    polygon.addPoint({1, 2});
    ASSERT_THROW(polygon.getPoint(10), std::out_of_range);
}


////////////////////////////////////////////////////////////
// Rectangle
////////////////////////////////////////////////////////////
class RectangleTest : public testing::Test
{
};

TEST_F(RectangleTest, shouldReturnCorrectSize_AfterConstructionBySide)
{
    const float side = 199.9;
    Rectangle rectangle(side);

    ASSERT_EQ(rectangle.getSize().width, side);
    ASSERT_EQ(rectangle.getSize().height, side);
}

TEST_F(RectangleTest, shouldReturnCorrectSize_AfterConstructionBySize)
{
    geometry::Size2Df expectedSize{122.3, 343.2};
    Rectangle rectangle(expectedSize);

    ASSERT_EQ(rectangle.getSize(), expectedSize);
}

TEST_F(RectangleTest, shouldReturnCorrectSize_AfterCopy)
{
    geometry::Size2Df expectedSize{122.3, 343.2};
    Rectangle rectangle1(expectedSize);
    Rectangle rectangle2(rectangle1);

    ASSERT_EQ(rectangle2.getSize(), expectedSize);
}

TEST_F(RectangleTest, whenGetNumOfPoints_shouldReturnCorrectValue)
{
    Rectangle rectangle(0.0);
    ASSERT_EQ(rectangle.getNumOfPoints(), 4);
}

TEST_F(RectangleTest, whenGetCenterPoint_shouldReturnCorrectValue)
{
    const geometry::Size2Df excpectedCenterPoint{155.55 * 0.5f, 175.255 * 0.5f};
    Rectangle rectangle({excpectedCenterPoint.width*2.0f, excpectedCenterPoint.height*2.0f});

    ASSERT_EQ(rectangle.getCenterPoint().x, excpectedCenterPoint.width);
    ASSERT_EQ(rectangle.getCenterPoint().y, excpectedCenterPoint.height);
}

class RectangleGetPointsTest : public testing::TestWithParam<std::pair<std::size_t, geometry::Point2Df>>
{
};

TEST_P(RectangleGetPointsTest, whenGetPointIndex_shouldReturnPointValue)
{
    Rectangle rectangle({3.0, 7.0});
    const auto [index, excpectValue] = GetParam();

    ASSERT_EQ(static_cast<int>(rectangle.getPoint(index).x * 1000), static_cast<int>(excpectValue.x * 1000));
    ASSERT_EQ(static_cast<int>(rectangle.getPoint(index).y * 1000), static_cast<int>(excpectValue.y * 1000));
}

INSTANTIATE_TEST_SUITE_P(
    RectangleGetPointsInstantiate, 
    RectangleGetPointsTest,
    testing::Values(
        std::make_pair(0, geometry::Point2Df{0.0, 0.0}),
        std::make_pair(1, geometry::Point2Df{3.0, 0.0}),
        std::make_pair(2, geometry::Point2Df{3.0, 7.0}),
        std::make_pair(3, geometry::Point2Df{0.0, 7.0})));

TEST_F(RectangleTest, whenGetPointAndValueOutOfRange_shouldThrowException)
{
    Rectangle rectangle(200);
    ASSERT_THROW(rectangle.getPoint(4), std::out_of_range);
}

class RectangleCheckPointInsideTest : public testing::TestWithParam<std::pair<geometry::Point2Df, bool>>
{
};

TEST_P(RectangleCheckPointInsideTest, whenCheckIsPointInside_shouldReturnStatement)
{
    Rectangle rectangle({3.0, 7.0});
    const auto [value, expectedStatement] = GetParam();

    ASSERT_EQ(rectangle.isPointInside(value), expectedStatement);
}

INSTANTIATE_TEST_SUITE_P(
    RectangleCheckPointInsideInstantiate, 
    RectangleCheckPointInsideTest,
    testing::Values(
        std::make_pair(geometry::Point2Df{0.0, 0.0}, true),
        std::make_pair(geometry::Point2Df{3.0, 0.0}, true),
        std::make_pair(geometry::Point2Df{3.0, 7.0}, true),
        std::make_pair(geometry::Point2Df{0.0, 7.0}, true),
        std::make_pair(geometry::Point2Df{1.0, 1.0}, true),
        std::make_pair(geometry::Point2Df{2.9, 7.1}, false)));

////////////////////////////////////////////////////////////
// Streched Hexagon
////////////////////////////////////////////////////////////
} // namespace geometry::figures