#pragma once

#include "geometry/transform/Transformable.hpp"
#include "geometry/transform/Transform.hpp"

namespace geometry
{
namespace transform
{

class Transformable2d : public Transformable
{
public:
    Transformable2d() = default;
    ~Transformable2d() = default;

public:
    void move(const geometry::Vector2Df& offset);
    void setOrigin(const Point2Df& origin);
    void setRotation(geometry::Angle angle);
    void setScale(const geometry::Vector2Df& factors);
    void setPosition(const Point2Df& position);

    const Point2Df& getOrigin() const;
    geometry::Angle getRotation() const;
    const Vector2Df& getScale() const;
    const Point2Df& getPosition() const;

public:
    const Transform& getTransform() const;
    Point2Df& transformPoint(Point2Df& point);
    Point2Df& inverseTransformPoint(Point2Df& point);

protected:    
    Point2Df& updatePoint(Point2Df& point);
    
private:
    void updateTransform();

protected:
    geometry::Vector2Df     m_scale{1.0f, 1.0f};
    geometry::Point2Df      m_origin;
    geometry::Point2Df      m_position;
    geometry::Angle         m_rotation;

    mutable geometry::transform::Transform               m_transform;
    mutable bool      m_transformNeedUpdate;        ///< Does the transform need to be recomputed?
    mutable bool m_inverseTransformNeedUpdate;
    // Transform      m_inverseTransform; // TODO or not
};
} // namespace transform
} // namespace geometry