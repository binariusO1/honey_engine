#pragma once

#include "gfx/geometry/Angle.hpp"
#include "gfx/geometry/transform/AxisOrder.hpp"
#include "gfx/geometry/transform/Transform.hpp"

namespace he
{
namespace gfx
{
namespace geometry
{
namespace transform
{
struct RotationArray
{
    he::gfx::geometry::Angle x{0.f};
    he::gfx::geometry::Angle y{0.f};
    he::gfx::geometry::Angle z{0.f};
    AxisOrder axisOrder{AxisOrder::XYZ};
};

template<typename POINT, typename VECTOR>
class ITransformable
{
public:
    virtual ~ITransformable() = default;

public:
    virtual void move(const VECTOR& offset) = 0;
    virtual bool setOrigin(const POINT& origin) = 0;
    virtual bool setPosition(const POINT& position) = 0;
    virtual bool setRotation(const float rotationZ) = 0;
    virtual bool setRotations(const float, const float, const float, const AxisOrder) = 0;
    virtual bool setScale(const VECTOR& factors) = 0;

    virtual const POINT& getOrigin() const = 0;
    virtual const POINT& getPosition() const = 0;
    virtual const geometry::Angle& getRotation() const = 0;
    virtual const RotationArray& getRotations() const = 0;
    virtual const VECTOR& getScale() const = 0;

    virtual const Transform& getTransform() const = 0;
    virtual void transformPoint(POINT& point) = 0;
    virtual void inverseTransformPoint(geometry::Point2Df& point) = 0;
};
} // namespace transform
} // namespace geometry
} // namespace gfx
} // namespace he