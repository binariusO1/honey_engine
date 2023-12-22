
#include <gtest/gtest.h>
#include "gfx/geometry/transform/Transformable.hpp"
#include "math/Matrix.hpp"

namespace  he::gfx::geometry::transform
{
class TransformableTests2d : public testing::Test
{
public:
    TransformableTests2d() = default;
    ~TransformableTests2d() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::geometry::transform::Transformable<geometry::Point2Df, geometry::Vector2Df>>();
    }
    std::unique_ptr<he::gfx::geometry::transform::Transformable<geometry::Point2Df, geometry::Vector2Df>> sut;

};

TEST_F(TransformableTests2d, whenCreateTransformFromMatrix_shouldCreateCorrectMatrix)
{
    createSut();
    const float matrix[16] = {
        1.f, 2.f, 3.f, 4.f,
        5.f, 6.f, 7.f, 8.f,
        9.f, 10.f, 11.f, 12.f,
        13.f, 14.f, 15.f, 16.f
    };
    const geometry::transform::Transform transform(matrix);

    for (auto i=0;i<16;++i)
    {
        ASSERT_EQ(transform.getMatrix()[i], i+1);
    }
}


TEST_F(TransformableTests2d, whenMoveThenGetNewPosition)
{
    createSut();
    sut->move({100.0, 100.0});
    ASSERT_EQ(sut->getPosition(), he::gfx::geometry::Point2Df(100.0, 100.0));
}

TEST_F(TransformableTests2d, whenSetOriginThenGetSameValue)
{
    createSut();
    sut->setOrigin({110.0, 110.0});
    ASSERT_EQ(sut->getOrigin(), he::gfx::geometry::Point2Df(110.0, 110.0));
}

TEST_F(TransformableTests2d, whenSetRotationThenGetSameValue)
{
    createSut();
    he::gfx::geometry::Angle angle(135.0);
    sut->setRotation(angle.asDegrees());
    ASSERT_EQ(sut->getRotation(), angle);
}

TEST_F(TransformableTests2d, whenSetScaleThenGetSameValue)
{
    createSut();
    sut->setScale({10.0, 1.1});
    ASSERT_EQ(sut->getScale(), he::gfx::geometry::Vector2Df(10.0, 1.1));
}

TEST_F(TransformableTests2d, whenSetPositionThenGetSameValue)
{
    createSut();
    sut->setPosition({115.0, 455.1});
    ASSERT_EQ(sut->getPosition(), he::gfx::geometry::Point2Df(115.0, 455.1));
}

TEST_F(TransformableTests2d, getDeterminant_whenGetDeterminant_shouldReturnCorrectValue)
{
    createSut();
    ASSERT_EQ(static_cast<int>(sut->getTransform().getDeterminant()), 1);
}

TEST_F(TransformableTests2d, setRotation_whenRotateAroundDefaultZAxis_shouldReturnCorrectValues)
{
    createSut();
    sut->setRotation(45);
    ASSERT_EQ(static_cast<int>(sut->getTransform().getMatrix()[0]*100000), 70710);
    ASSERT_EQ(static_cast<int>(sut->getTransform().getMatrix()[1]*100000), -70710);
    ASSERT_EQ(static_cast<int>(sut->getTransform().getMatrix()[4]*100000), 70710);
    ASSERT_EQ(static_cast<int>(sut->getTransform().getMatrix()[5]*100000), 70710);
}

TEST_F(TransformableTests2d, rotateAroundZ_whenRotatePointAroundZ_shouldTransformPoint)
{
    createSut();
    geometry::Point2Df pointToTransform({200.0, 0.0});
    sut->setRotation(-45);
    math::transformPoint2d(pointToTransform.x, pointToTransform.y, sut->getTransform().getMatrix());
    ASSERT_EQ(static_cast<int>(pointToTransform.x), 141);
    ASSERT_EQ(static_cast<int>(pointToTransform.y), 141);
}

TEST_F(TransformableTests2d, checkPointPosition_whenSetPosition)
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

TEST_F(TransformableTests2d, checkInverseTransformPoint)
{
    Transformable<geometry::Point2Df, geometry::Vector2Df> transformableObject;
    Point2Df point({100, 100});
    const Point2Df pointBeforeTransform = point;

    transformableObject.setOrigin({75, 75});
    transformableObject.setPosition({200, 200});
    transformableObject.setScale({0.5f, 0.5f});
    transformableObject.setRotation(45);
    transformableObject.transformPoint(point);

    ASSERT_NE(static_cast<int>(point.x), static_cast<int>(pointBeforeTransform.x));
    ASSERT_NE(static_cast<int>(point.y), static_cast<int>(pointBeforeTransform.y));

    transformableObject.inverseTransformPoint(point);

    ASSERT_EQ(static_cast<int>((point.x)), static_cast<int>(pointBeforeTransform.x));
    ASSERT_EQ(static_cast<int>((point.y)), static_cast<int>(pointBeforeTransform.y));
}

class TransformableTests3d : public testing::Test
{
public:
    TransformableTests3d() = default;
    ~TransformableTests3d() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::geometry::transform::Transformable<geometry::Point3Df, geometry::Vector3Df>>();
    }
    std::unique_ptr<he::gfx::geometry::transform::Transformable<geometry::Point3Df, geometry::Vector3Df>> sut;

};

TEST_F(TransformableTests3d, whenMoveThenGetNewPosition)
{
    createSut();
    sut->move({100.0, 200.0, 300.0});
    ASSERT_EQ(sut->getPosition(), he::gfx::geometry::Point3Df(100.0, 200.0, 300.0));
}

TEST_F(TransformableTests3d, whenSetOriginThenGetSameValue)
{
    createSut();
    sut->setOrigin({110.0, 120.0, 130.0});
    ASSERT_EQ(sut->getOrigin(), he::gfx::geometry::Point3Df(110.0, 120.0, 130.0));
}

TEST_F(TransformableTests3d, whenSetRotationThenGetSameValueForAllAxis)
{
    createSut();
    he::gfx::geometry::Angle angle1(135.0);
    he::gfx::geometry::Angle angle2(23.0);
    he::gfx::geometry::Angle angle3(235.0);

    sut->setRotations(angle1.asDegrees(), angle2.asDegrees(), angle3.asDegrees(), AxisOrder::XYZ);
    ASSERT_EQ(sut->getRotations().x, angle1);
    ASSERT_EQ(sut->getRotations().y, angle2);
    ASSERT_EQ(sut->getRotations().z, angle3);
}

TEST_F(TransformableTests3d, whenSetScaleThenGetSameValue)
{
createSut();
    sut->setScale({10.0, 1.1, 3.5});
    ASSERT_EQ(sut->getScale(), he::gfx::geometry::Vector3Df(10.0, 1.1, 3.5));
}

TEST_F(TransformableTests3d, whenSetPositionThenGetSameValue)
{
    createSut();
    sut->setPosition({115.0, 455.1, 2.2});
    ASSERT_EQ(sut->getPosition(), he::gfx::geometry::Point3Df(115.0, 455.1, 2.2));
}

TEST_F(TransformableTests3d, setRotation_whenRotateAroundDefaultXAxis_shouldReturnCorrectValues)
{
    createSut();
    sut->setRotations(45, 0, 0, AxisOrder::XYZ);
    ASSERT_EQ(static_cast<int>(sut->getTransform().getMatrix()[5]*100000), 70710);
    ASSERT_EQ(static_cast<int>(sut->getTransform().getMatrix()[6]*100000), 70710);
    ASSERT_EQ(static_cast<int>(sut->getTransform().getMatrix()[9]*100000), -70710);
    ASSERT_EQ(static_cast<int>(sut->getTransform().getMatrix()[10]*100000), 70710);
}

TEST_F(TransformableTests3d, setRotation_whenRotateAroundDefaultYAxis_shouldReturnCorrectValues)
{
    createSut();
    sut->setRotations(0, 45, 0, AxisOrder::XYZ);
    ASSERT_EQ(static_cast<int>(sut->getTransform().getMatrix()[0]*100000), 70710);
    ASSERT_EQ(static_cast<int>(sut->getTransform().getMatrix()[2]*100000), -70710);
    ASSERT_EQ(static_cast<int>(sut->getTransform().getMatrix()[8]*100000), 70710);
    ASSERT_EQ(static_cast<int>(sut->getTransform().getMatrix()[10]*100000), 70710);
}

TEST_F(TransformableTests3d, rotateAroundZ_whenRotatePointAroundZ_shouldTransformPoint)
{
    createSut();
    geometry::Point3Df pointToTransform({200.0, 10.0, 100.0});
    sut->setRotations(0, 0, 45, AxisOrder::XYZ);
    math::transformPoint3d(pointToTransform.x, pointToTransform.y, pointToTransform.z, sut->getTransform().getMatrix());
    ASSERT_EQ(static_cast<int>(pointToTransform.x), 148);
    ASSERT_EQ(static_cast<int>(pointToTransform.y), -134);
    ASSERT_EQ(static_cast<int>(pointToTransform.z), 100);
}

TEST_F(TransformableTests3d, rotateAroundX_whenRotatePointAroundX_shouldTransformPoint)
{
    createSut();
    geometry::Point3Df pointToTransform({200.0, 10.0, 100.0});
    sut->setRotations(45, 0, 0, AxisOrder::XYZ);
    math::transformPoint3d(pointToTransform.x, pointToTransform.y, pointToTransform.z, sut->getTransform().getMatrix());
    ASSERT_EQ(static_cast<int>(pointToTransform.x), 200);
    ASSERT_EQ(static_cast<int>(pointToTransform.y*10), -636);
    ASSERT_EQ(static_cast<int>(pointToTransform.z*100), 7778);
}

TEST_F(TransformableTests3d, rotateAroundY_whenRotatePointAroundY_shouldTransformPoint)
{
    createSut();
    geometry::Point3Df pointToTransform({200.0, 10.0, 100.0});
    sut->setRotations(0, -45, 0, AxisOrder::XYZ);
    math::transformPoint3d(pointToTransform.x, pointToTransform.y, pointToTransform.z, sut->getTransform().getMatrix());
    ASSERT_EQ(static_cast<int>(pointToTransform.x*100), 7071);
    ASSERT_EQ(static_cast<int>(pointToTransform.y), 10);
    ASSERT_EQ(static_cast<int>(pointToTransform.z*100), 21213);
}

TEST_F(TransformableTests3d, rotateAroundXY_whenRotatePointAroundXY_shouldTransformPoint)
{
    createSut();
    geometry::Point3Df pointToTransform({200.0, 10.0, 100.0});
    sut->setRotations(45, 30, 0, AxisOrder::XYZ);
    math::transformPoint3d(pointToTransform.x, pointToTransform.y, pointToTransform.z, sut->getTransform().getMatrix());
    ASSERT_EQ(static_cast<int>(pointToTransform.x*100), 21209);
    ASSERT_EQ(static_cast<int>(pointToTransform.y*10), -636);
    ASSERT_EQ(static_cast<int>(pointToTransform.z*100), -3263);
}

TEST_F(TransformableTests3d, rotateAroundYX_whenRotatePointAroundYX_shouldTransformPoint)
{
    createSut();
    geometry::Point3Df pointToTransform({200.0, 10.0, 100.0});
    sut->setRotations(45, 30, 0, AxisOrder::YXZ);
    math::transformPoint3d(pointToTransform.x, pointToTransform.y, pointToTransform.z, sut->getTransform().getMatrix());
    ASSERT_EQ(static_cast<int>(pointToTransform.x*100), 22320);
    ASSERT_EQ(static_cast<int>(pointToTransform.y*100), 1654);
    ASSERT_EQ(static_cast<int>(pointToTransform.z*100), -240);
}

TEST_F(TransformableTests3d, rotateAroundXYZ_whenRotatePointAroundXYZ_shouldTransformPoint)
{
    createSut();
    geometry::Point3Df pointToTransform({200.0, 10.0, 100.0});
    sut->setRotations(45, 30, 45, AxisOrder::XYZ);
    math::transformPoint3d(pointToTransform.x, pointToTransform.y, pointToTransform.z, sut->getTransform().getMatrix());
    ASSERT_EQ(static_cast<int>(pointToTransform.x*100), 10497);
    ASSERT_EQ(static_cast<int>(pointToTransform.y*100), -19497);
    ASSERT_EQ(static_cast<int>(pointToTransform.z*100), -3263);
}

TEST_F(TransformableTests3d, rotateAroundXZY_whenRotatePointAroundXZY_shouldTransformPoint)
{
    createSut();
    geometry::Point3Df pointToTransform({200.0, 10.0, 100.0});
    sut->setRotations(45, 30, 45, AxisOrder::XZY);
    math::transformPoint3d(pointToTransform.x, pointToTransform.y, pointToTransform.z, sut->getTransform().getMatrix());
    ASSERT_EQ(static_cast<int>(pointToTransform.x*100), 12239);
    ASSERT_EQ(static_cast<int>(pointToTransform.y*100), -18642);
    ASSERT_EQ(static_cast<int>(pointToTransform.z*100), 1915);
}

TEST_F(TransformableTests3d, rotateAroundYXZ_whenRotatePointAroundYXZ_shouldTransformPoint)
{
    createSut();
    geometry::Point3Df pointToTransform({200.0, 10.0, 100.0});
    sut->setRotations(45, 30, 45, AxisOrder::YXZ);
    math::transformPoint3d(pointToTransform.x, pointToTransform.y, pointToTransform.z, sut->getTransform().getMatrix());
    ASSERT_EQ(static_cast<int>(pointToTransform.x*100), 16952);
    ASSERT_EQ(static_cast<int>(pointToTransform.y*100), -14613);
    ASSERT_EQ(static_cast<int>(pointToTransform.z*100), -240);
}

TEST_F(TransformableTests3d, rotateAroundYZX_whenRotatePointAroundYZX_shouldTransformPoint)
{
    createSut();
    geometry::Point3Df pointToTransform({200.0, 10.0, 100.0});
    sut->setRotations(45, 30, 45, AxisOrder::YZX);
    math::transformPoint3d(pointToTransform.x, pointToTransform.y, pointToTransform.z, sut->getTransform().getMatrix());
    ASSERT_EQ(static_cast<int>(pointToTransform.x*100), 16490);
    ASSERT_EQ(static_cast<int>(pointToTransform.y*100), -9712);
    ASSERT_EQ(static_cast<int>(pointToTransform.z*100), -11607);
}

TEST_F(TransformableTests3d, rotateAroundZXY_whenRotatePointAroundZXY_shouldTransformPoint)
{
    createSut();
    geometry::Point3Df pointToTransform({200.0, 10.0, 100.0});
    sut->setRotations(45, 30, 45, AxisOrder::ZXY);
    math::transformPoint3d(pointToTransform.x, pointToTransform.y, pointToTransform.z, sut->getTransform().getMatrix());
    ASSERT_EQ(static_cast<int>(pointToTransform.x*100), 11645);
    ASSERT_EQ(static_cast<int>(pointToTransform.y*100), -16571);
    ASSERT_EQ(static_cast<int>(pointToTransform.z*100), -9528);
}

TEST_F(TransformableTests3d, rotateAroundZYX_whenRotatePointAroundZYX_shouldTransformPoint)
{
    createSut();
    geometry::Point3Df pointToTransform({200.0, 10.0, 100.0});
    sut->setRotations(45, 30, 45, AxisOrder::ZYX);
    math::transformPoint3d(pointToTransform.x, pointToTransform.y, pointToTransform.z, sut->getTransform().getMatrix());
    ASSERT_EQ(static_cast<int>(pointToTransform.x*100), 17859);
    ASSERT_EQ(static_cast<int>(pointToTransform.y*100), -10373);
    ASSERT_EQ(static_cast<int>(pointToTransform.z*100), -8626);
}
} // namespace he::gfx::geometry::transform