#pragma once

#include "geometry/Point2d.hpp"
#include "geometry/Vector2d.hpp"
#include "geometry/Angle.hpp"

namespace geometry
{
namespace transform
{

class Transformable
{
public:
    Transformable() = default;
    virtual ~Transformable() = default;

public:
    virtual void move(const Vector2Df& offset) = 0;
    virtual void setOrigin(const Point2Df& origin) = 0;
    virtual void setRotation(geometry::Angle angle) = 0;
    virtual void setScale(const geometry::Vector2Df& factors) = 0;
    virtual void setPosition(const Point2Df& position) = 0;

    virtual const Point2Df& getOrigin() const = 0;
    virtual geometry::Angle getRotation() const = 0;
    virtual const geometry::Vector2Df& getScale() const = 0;
    virtual const Point2Df& getPosition() const = 0;
};
} // namespace transform
} // namespace geometry