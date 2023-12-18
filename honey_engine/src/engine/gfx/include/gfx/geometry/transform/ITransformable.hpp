#pragma once

#include "gfx/geometry/Angle.hpp"
#include "gfx/geometry/transform/Transform.hpp"

namespace he
{
namespace gfx
{
namespace geometry
{
namespace transform
{

template<typename POINT, typename VECTOR>
class ITransformable
{
public:
    virtual ~ITransformable() = default;

public:
    virtual void move(const VECTOR& offset) = 0;
    virtual bool setOrigin(const POINT& origin) = 0;
    virtual bool setPosition(const POINT& position) = 0;
    virtual bool setRotation(const geometry::Angle& angle, const int) = 0;
    virtual bool setScale(const VECTOR& factors) = 0;

    virtual const POINT& getOrigin() const = 0;
    virtual const POINT& getPosition() const = 0;
    virtual const he::gfx::geometry::Angle& getRotation(const int) const = 0;
    virtual const VECTOR& getScale() const = 0;

    virtual const Transform& getTransform() const = 0;
    virtual void transformPoint(POINT& point) = 0;
    virtual void inverseTransformPoint(POINT& point) = 0;
};
} // namespace transform
} // namespace geometry
} // namespace gfx
} // namespace he