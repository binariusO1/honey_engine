#pragma once

#include "gfx/geometry/Point2d.hpp"
#include "gfx/geometry/Vector2d.hpp"
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

class ITransformable
{
public:
    virtual ~ITransformable() = default;

public:
    virtual void move(const Vector2Df& offset) = 0;
    virtual void setOrigin(const Point2Df& origin) = 0;
    virtual void setOriginInCenter() = 0;
    virtual void setPosition(const Point2Df& position) = 0;
    virtual void setRotation(const geometry::Angle& angle) = 0;
    virtual void setScale(const geometry::Vector2Df& factors) = 0;

    virtual const Point2Df& getOrigin() const = 0;
    virtual const Point2Df& getPosition() const = 0;
    virtual const he::gfx::geometry::Angle& getRotation() const = 0;
    virtual const he::gfx::geometry::Vector2Df& getScale() const = 0;

    virtual const Transform& getTransform() const = 0;
    virtual void transformPoint(Point2Df& point) = 0;
    virtual void inverseTransformPoint(Point2Df& point) = 0;
};
} // namespace transform
} // namespace geometry
} // namespace gfx
} // namespace he