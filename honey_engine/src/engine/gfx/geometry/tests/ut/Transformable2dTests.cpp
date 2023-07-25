
#include <gtest/gtest.h>
#include "geometry/transform/Transformable2d.hpp"

namespace  geometry::transform
{
class Transformable2dTests : public testing::Test
{
public:

};

TEST_F(Transformable2dTests, checkInverseTransformPoint)
{
    Transformable2d transformableObject;
    Point2Df point({100, 100});
    Point2Df pointBeforeTransform = point;

    transformableObject.setOrigin({75, 75});
    transformableObject.setPosition({200, 200});
    transformableObject.setScale({0.5f, 0.5f});
    transformableObject.setRotation(Angle(45.0f));
    transformableObject.transformPoint(point);
    transformableObject.inverseTransformPoint(point);

    ASSERT_EQ(static_cast<int>(point.x), static_cast<int>(pointBeforeTransform.x));
    ASSERT_EQ(static_cast<int>(point.y), static_cast<int>(pointBeforeTransform.y));
}
} // namespace geometry::transform