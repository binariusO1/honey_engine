
#include <gtest/gtest.h>
#include "geometry/figures/Cross.hpp"
#include "geometry/figures/Hexagon.hpp"
#include "geometry/figures/Polygon.hpp"
#include "geometry/figures/Rectangle.hpp"

namespace  geometry::figures
{
class CrossTests : public testing::Test
{
};

TEST_F(CrossTests, whenGetPointValueOverflow_shouldReturnDefaultValueOfPointAndPrintWarning)
{
    Cross cross(200);
    geometry::Point2Df point = cross.getPoint(10);
    geometry::Point2Df expectedPoint{0, 0};
    ASSERT_EQ(point, expectedPoint);
}


class HexagonTests : public testing::Test
{
};

TEST_F(HexagonTests, whenGetPointValueOverflow_shouldReturnDefaultValueOfPointAndPrintWarning)
{
    Hexagon hexagon(300);
    geometry::Point2Df point = hexagon.getPoint(10);
    geometry::Point2Df expectedPoint{0, 0};
    ASSERT_EQ(point, expectedPoint);
}

TEST_F(HexagonTests, whenCheckPointInside_shouldReturnTrue)
{
    Hexagon hexagon(300);
    geometry::Point2Df point{300, 300};
    ASSERT_EQ(true, hexagon.isPointInside(point));
}

TEST_F(HexagonTests, whenCheckPointOnBorder_shouldReturnTrue)
{
    Hexagon hexagon(200);
    geometry::Point2Df point1{300, 0};
    geometry::Point2Df point2{100, 0};
    ASSERT_EQ(true, hexagon.isPointInside(point1));
    ASSERT_EQ(true, hexagon.isPointInside(point2));
}

TEST_F(HexagonTests, whenCheckPointoutside_shouldReturnFalse)
{
    Hexagon hexagon(200);
    geometry::Point2Df point{99, 0};
    ASSERT_EQ(false, hexagon.isPointInside(point));
}


class PolygonTests : public testing::Test
{
};

TEST_F(PolygonTests, whenGetPointValueOverflow_shouldReturnDefaultValueOfPointAndPrintWarning)
{
    Polygon polygon;
    polygon.addPoint({1, 2});
    geometry::Point2Df point = polygon.getPoint(10);
    geometry::Point2Df expectedPoint{0, 0};
    ASSERT_EQ(point, expectedPoint);
}


class RectangleTests : public testing::Test
{
};

TEST_F(RectangleTests, whenGetPointValueOverflow_shouldReturnDefaultValueOfPointAndPrintWarning)
{
    Rectangle rectangle(200);
    geometry::Point2Df point = rectangle.getPoint(10);
    geometry::Point2Df expectedPoint{0, 0};
    ASSERT_EQ(point, expectedPoint);
}

TEST_F(RectangleTests, whenCheckPointInside_shouldReturnTrue)
{
    Rectangle rectangle({300, 200});
    geometry::Point2Df point{1, 199};
    ASSERT_EQ(true, rectangle.isPointInside(point));
}

TEST_F(RectangleTests, whenCheckPointOnBorder_shouldReturnTrue)
{
    Rectangle rectangle({300, 200});
    geometry::Point2Df point{300, 200};
    ASSERT_EQ(true, rectangle.isPointInside(point));
}

TEST_F(RectangleTests, whenCheckPointoutside_shouldReturnFalse)
{
    Rectangle rectangle({150, 700});
    geometry::Point2Df point{151, 701};
    ASSERT_EQ(false, rectangle.isPointInside(point));
}

// TODO MORE TESTS
} // namespace geometry::figures