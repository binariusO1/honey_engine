
#include <gtest/gtest.h>
#include "geometry/figures/Cross.hpp"
#include "geometry/figures/Hexagon.hpp"
#include "geometry/figures/Polygon.hpp"
#include "geometry/figures/Rectangle.hpp"
#include "geometry/figures/StretchedHexagon.hpp"

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

TEST_F(PolygonTest, whenGetNumOfPoints_shouldReturnCorrectValue)
{
    Polygon polygon;
    std::list<geometry::Point2Df> points{{0.0, 1.1}, {2.3, 3.4}, {2.3, 5.3}};
    polygon.setPoints(points);

    ASSERT_EQ(polygon.getNumOfPoints(), 3);
}

TEST_F(PolygonTest, whenGetNumOfPointsAfterAdded_shouldReturnCorrectValue)
{
    Polygon polygon;
    polygon.addPoint({0.0, 1.1});
    polygon.addPoint({0.0, 1.1});

    ASSERT_EQ(polygon.getNumOfPoints(), 2);
}

TEST_F(PolygonTest, whenGetNumOfPointsAfterCreate_shouldReturnCorrectValue)
{
    Polygon polygon;

    ASSERT_EQ(polygon.getNumOfPoints(), 0);
}

class PolygonGetCenterPointTest : public testing::TestWithParam<std::pair<geometry::Point2Df, std::list<geometry::Point2Df>>>
{
};

TEST_P(PolygonGetCenterPointTest, whenGetPointIndex_shouldReturnPointValue)
{
    const auto [centerPoint, points] = GetParam();

    Polygon polygon;
    polygon.setPoints(points);

    ASSERT_EQ(static_cast<int>(polygon.getCenterPoint().x * 1000), static_cast<int>(centerPoint.x * 1000));
    ASSERT_EQ(static_cast<int>(polygon.getCenterPoint().y * 1000), static_cast<int>(centerPoint.y * 1000));
}

INSTANTIATE_TEST_SUITE_P(
    PolygonGetCenterPointInstantiate, 
    PolygonGetCenterPointTest,
    testing::Values(
        std::make_pair(geometry::Point2Df{7.77, 11.25}, std::list<geometry::Point2Df>{{7.77, 11.25}}),
        std::make_pair(geometry::Point2Df{2.0, 1.5}, std::list<geometry::Point2Df>{{1.0, 1.0}, {3.0, 2.0}}),
        std::make_pair(geometry::Point2Df{2.0, 5.333}, std::list<geometry::Point2Df>{{0.5, 1.1}, {2.2, 10.5}, {3.3, 4.4}}),
        std::make_pair(geometry::Point2Df{1.5, 1.5}, std::list<geometry::Point2Df>{{1.0, 1.0}, {2.0, 1.0}, {2.0, 2.0}, {1.0, 2.0}}),
        std::make_pair(geometry::Point2Df{7.851, 9.401}, std::list<geometry::Point2Df>{{13.3, 1.1}, {14.1, 15.4}, {3.4, 17.4}, {0.1, 2.7}, {1.44, 4.5}})
        ));

class PolygonGetPointsTest : public testing::TestWithParam<std::pair<std::size_t, geometry::Point2Df>>
{
};

TEST_P(PolygonGetPointsTest, whenGetPointIndex_shouldReturnPointValue)
{
    Polygon polygon;
    polygon.setPoints({{13.3, 1.1}, {14.1, 15.4}, {3.4, 17.4}, {0.1, 2.7}, {1.44, 4.5}, {0.2, 0.0}});
    const auto [index, excpectValue] = GetParam();

    ASSERT_EQ(static_cast<int>(polygon.getPoint(index).x * 1000), static_cast<int>(excpectValue.x * 1000));
    ASSERT_EQ(static_cast<int>(polygon.getPoint(index).y * 1000), static_cast<int>(excpectValue.y * 1000));
}

INSTANTIATE_TEST_SUITE_P(
    PolygonGetPointsInstantiate, 
    PolygonGetPointsTest,
    testing::Values(
        std::make_pair(0, geometry::Point2Df{13.3, 1.1}),
        std::make_pair(1, geometry::Point2Df{14.1, 15.4}),
        std::make_pair(2, geometry::Point2Df{3.4, 17.4}),
        std::make_pair(3, geometry::Point2Df{0.1, 2.7}),
        std::make_pair(4, geometry::Point2Df{1.44, 4.5}),
        std::make_pair(5, geometry::Point2Df{0.2, 0.0})));

TEST_F(PolygonTest, whenGetPointAndValueOutOfRange_shouldThrowException)
{
    Polygon polygon;
    polygon.addPoint({1, 2});
    ASSERT_THROW(polygon.getPoint(10), std::out_of_range);
}

class PolygonCheckPointInsideTest : public testing::TestWithParam<std::pair<geometry::Point2Df, bool>>
{
};

TEST_P(PolygonCheckPointInsideTest, whenCheckIsPointInside_shouldReturnStatement)
{
    Polygon polygon;
    polygon.setPoints({{0.0, 0.0}, {1.236, 4.176}, {7.099, 11.144}, {10.462, 5.185}, {5.225, 5.57}, {4.433, 2.895}, {11.583, 0.409}, {3.238, 0.818}});
    const auto [value, expectedStatement] = GetParam();

    ASSERT_EQ(polygon.isPointInside(value), expectedStatement);
}

INSTANTIATE_TEST_SUITE_P(
    PolygonCheckPointInsideInstantiate, 
    PolygonCheckPointInsideTest,
    testing::Values(
        std::make_pair(geometry::Point2Df{2.21, 4.89}, true),
        std::make_pair(geometry::Point2Df{7.411, 8.778}, true),
        std::make_pair(geometry::Point2Df{2.634, 0.236}, false),
        std::make_pair(geometry::Point2Df{4.433, 2.895}, true),
        std::make_pair(geometry::Point2Df{6.129, 4.588}, false),
        std::make_pair(geometry::Point2Df{10.462, 1.445}, false)));

TEST_F(PolygonTest, shouldAddAndRemovePoint)
{
    Polygon polygon;
    polygon.addPoint({0.0, 0.0});
    polygon.addPoint({1.0, 1.0});
    polygon.removePoint(1);
    ASSERT_EQ(polygon.getNumOfPoints(), 1);
}

TEST_F(PolygonTest, whenGetNegativeNumberIndexDuringRemove_shouldThrowException)
{
    Polygon polygon;
    polygon.addPoint({0.0, 0.0});
    ASSERT_THROW(polygon.removePoint(-1), std::out_of_range);
}

TEST_F(PolygonTest, whenGetWrongIndexDuringRemove_shouldThrowException)
{
    Polygon polygon;
    polygon.addPoint({0.0, 0.0});
    ASSERT_THROW(polygon.removePoint(2), std::out_of_range);
}

TEST_F(PolygonTest, whenGetPoints_shouldReturnPoints)
{
    Polygon polygon;
    std::list<geometry::Point2Df> listOfPoints{{0.0, 0.0}, {1.236, 4.176}, {7.099, 11.144}, {10.462, 5.185}, {5.225, 5.57}, {4.433, 2.895}, {11.583, 0.409}, {3.238, 0.818}};
    polygon.setPoints(listOfPoints);

    ASSERT_EQ(polygon.getPoints(), listOfPoints);
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

class StretchedHexagonTest : public testing::Test
{
};

TEST_F(StretchedHexagonTest, shouldReturnCorrectSize_AfterCopy)
{
    geometry::Size2Df expectedSize{122.3, 343.2};
    StretchedHexagon stretchedHexagon1(expectedSize);
    StretchedHexagon stretchedHexagon2(stretchedHexagon1);

    ASSERT_EQ(stretchedHexagon2.getSize(), expectedSize);
}

TEST_F(StretchedHexagonTest, whenGetNumOfPoints_shouldReturnCorrectValue)
{
    StretchedHexagon stretchedHexagon({10.0, 4.0});

    ASSERT_EQ(stretchedHexagon.getNumOfPoints(), 6);
}

TEST_F(StretchedHexagonTest, whenGetCenterPoint_shouldReturnCorrectValue)
{
    StretchedHexagon stretchedHexagon({10.0, 4.0});
    const geometry::Point2Df excpectedCenterPoint{5.0, 2.0};

    ASSERT_EQ(stretchedHexagon.getCenterPoint().x, excpectedCenterPoint.x);
    ASSERT_EQ(stretchedHexagon.getCenterPoint().y, excpectedCenterPoint.y);
}

class StretchedHexagonGetPointsTest : public testing::TestWithParam<std::pair<std::size_t, geometry::Point2Df>>
{
};

TEST_P(StretchedHexagonGetPointsTest, whenGetPointIndex_shouldReturnPointValue)
{
    StretchedHexagon stretchedHexagon({10.0, 4.0});
    const auto [index, excpectValue] = GetParam();

    ASSERT_EQ(static_cast<int>(stretchedHexagon.getPoint(index).x * 1000), static_cast<int>(excpectValue.x * 1000));
    ASSERT_EQ(static_cast<int>(stretchedHexagon.getPoint(index).y * 1000), static_cast<int>(excpectValue.y * 1000));
}

INSTANTIATE_TEST_SUITE_P(
    StretchedHexagonGetPointsInstantiate, 
    StretchedHexagonGetPointsTest,
    testing::Values(
        std::make_pair(0, geometry::Point2Df{0.0, 2.0}),
        std::make_pair(1, geometry::Point2Df{1.154, 0.0}),
        std::make_pair(2, geometry::Point2Df{8.845, 0.0}),
        std::make_pair(3, geometry::Point2Df{10.0, 2.0}),
        std::make_pair(4, geometry::Point2Df{8.845, 4.0}),
        std::make_pair(5, geometry::Point2Df{1.154, 4.0})));

TEST_F(StretchedHexagonTest, whenGetPointAndValueOutOfRange_shouldThrowException)
{
    StretchedHexagon stretchedHexagon({10.0, 6.0});
    ASSERT_THROW(stretchedHexagon.getPoint(6), std::out_of_range);
}

class StretchedHexagonCheckPointInsideTest : public testing::TestWithParam<std::pair<geometry::Point2Df, bool>>
{
};

TEST_P(StretchedHexagonCheckPointInsideTest, whenCheckIsPointInside_shouldReturnStatement)
{
    StretchedHexagon stretchedHexagon({10.0, 4.0});
    const auto [value, expectedStatement] = GetParam();

    ASSERT_EQ(stretchedHexagon.isPointInside(value), expectedStatement);
}

INSTANTIATE_TEST_SUITE_P(
    StretchedHexagonCheckPointInsideInstantiate, 
    StretchedHexagonCheckPointInsideTest,
    testing::Values(
        std::make_pair(geometry::Point2Df{0.815, 3.707}, false),
        std::make_pair(geometry::Point2Df{3.262, 3.036}, true),
        std::make_pair(geometry::Point2Df{7.155, 2.78}, true),
        std::make_pair(geometry::Point2Df{8.845, 4.0}, true),
        std::make_pair(geometry::Point2Df{10.0, 2.0}, true),
        std::make_pair(geometry::Point2Df{8.998, 0.132}, false)));

TEST_F(StretchedHexagonTest, shouldReturnCorrectSize_AfterSetSize)
{
    geometry::Size2Df size{1200.3, 1203.2};
    StretchedHexagon stretchedHexagon;
    stretchedHexagon.setSize(size);

    ASSERT_EQ(stretchedHexagon.getSize(), size);
}
} // namespace geometry::figures