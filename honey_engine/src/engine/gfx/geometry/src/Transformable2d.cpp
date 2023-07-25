#include "geometry/transform/Transformable2d.hpp"
#include <cmath>

namespace geometry
{
namespace transform
{


////////////////////////////////////////////////////////////
void Transformable2d::move(const geometry::Vector2Df& offset)
{
    setPosition({m_position.x + offset.x , m_position.y + offset.y});
}


////////////////////////////////////////////////////////////
void Transformable2d::setOrigin(const Point2Df& origin)
{
    m_origin = origin;
    m_transformNeedUpdate        = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable2d::setRotation(Angle angle)
{
    m_rotation = angle.wrapUnsigned();
    m_transformNeedUpdate        = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable2d::setScale(const geometry::Vector2Df& factors)
{
    m_scale = factors;
    m_transformNeedUpdate        = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable2d::setPosition(const Point2Df& position)
{
    m_position = position;
    m_transformNeedUpdate        = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
const Point2Df& Transformable2d::getOrigin() const
{
    return m_origin;
}


////////////////////////////////////////////////////////////
Angle Transformable2d::getRotation() const
{
    return m_rotation;
}


////////////////////////////////////////////////////////////
const geometry::Vector2Df& Transformable2d::getScale() const
{
    return m_scale;
}


////////////////////////////////////////////////////////////
const Point2Df& Transformable2d::getPosition() const
{
    return m_position;
}


//PROTECTED
////////////////////////////////////////////////////////////
Point2Df& Transformable2d::updatePoint(Point2Df& point)
{
    point += m_position;
    return point;
}


////////////////////////////////////////////////////////////
Point2Df& Transformable2d::transformPoint(Point2Df& point)
{
    float angle  = -m_rotation.asRadians();
    float cosine = std::cos(angle);
    float sine   = std::sin(angle);
    float sxc    = m_scale.x * cosine;
    float syc    = m_scale.y * cosine;
    float sxs    = m_scale.x * sine;
    float sys    = m_scale.y * sine;

    float ox = point.x - m_origin.x;
    float oy = point.y - m_origin.y;
    point.x = ox * sxc - oy * sys; 
    point.y = ox * sxs + oy * syc;

    updatePoint(point);
    return point;
}


////////////////////////////////////////////////////////////
Point2Df& Transformable2d::inverseTransformPoint(Point2Df& point)
{
    point -= m_position;

    float angle  = -m_rotation.asRadians();
    float cosine = std::cos(angle);
    float sine   = std::sin(angle);

    float ox = point.x;
    float oy = point.y;
    point.x = ox * cosine + oy * sine; 
    point.y = oy * cosine - ox * sine;

    point.x *= (1.0f / m_scale.x);
    point.y *= (1.0f / m_scale.y);

    point.x += m_origin.x;
    point.y += m_origin.y;
    return point;
}


////////////////////////////////////////////////////////////
void Transformable2d::updateTransform()
{
    //
}


////////////////////////////////////////////////////////////
const Transform& Transformable2d::getTransform() const
{
    // Recompute the combined transform if needed
    if (m_transformNeedUpdate)
    {
        float angle  = -m_rotation.asRadians();
        float cosine = std::cos(angle);
        float sine   = std::sin(angle);
        float sxc    = m_scale.x * cosine;
        float syc    = m_scale.y * cosine;
        float sxs    = m_scale.x * sine;
        float sys    = m_scale.y * sine;
        float tx     = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
        float ty     = m_origin.x * sxs - m_origin.y * syc + m_position.y;

        // clang-format off
        m_transform = Transform( sxc, sys, tx,
                                -sxs, syc, ty,
                                 0.f, 0.f, 1.f);
        // clang-format on
        m_transformNeedUpdate = false;
    }

    return m_transform;
}
} // namespace transform
} // namespace geometry