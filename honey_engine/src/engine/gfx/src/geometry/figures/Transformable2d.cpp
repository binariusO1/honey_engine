#include "gfx/geometry/transform/Transformable2d.hpp"

#include <cmath>

namespace he
{
namespace gfx
{
namespace geometry
{
namespace transform
{
////////////////////////////////////////////////////////////
Transformable2d::~Transformable2d() = default;


////////////////////////////////////////////////////////////
void Transformable2d::move(const geometry::Vector2Df& offset)
{
    setPosition({m_position.x + offset.x , m_position.y + offset.y});
}


////////////////////////////////////////////////////////////
bool Transformable2d::setPosition(const Point2Df& position)
{
    m_position = position;
    m_transformNeedUpdate = (m_position == position);
    return m_transformNeedUpdate;
}


////////////////////////////////////////////////////////////
bool Transformable2d::setOrigin(const Point2Df& origin)
{
    m_origin = origin;
    m_transformNeedUpdate = (m_origin == origin);
    return m_transformNeedUpdate;
}


////////////////////////////////////////////////////////////
bool Transformable2d::setScale(const geometry::Vector2Df& factors)
{
    m_scale = factors;
    m_transformNeedUpdate = (m_scale == factors);
    return m_transformNeedUpdate;
}


////////////////////////////////////////////////////////////
bool Transformable2d::setRotation(const Angle& angle)
{
    m_rotation = angle.wrapUnsigned();
    m_transformNeedUpdate = (m_rotation == angle.wrapUnsigned());
    return m_transformNeedUpdate;
}


////////////////////////////////////////////////////////////
const Point2Df& Transformable2d::getOrigin() const
{
    return m_origin;
}


////////////////////////////////////////////////////////////
const Angle& Transformable2d::getRotation() const
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


////////////////////////////////////////////////////////////
//PROTECTED
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
void Transformable2d::transformPoint(Point2Df& point)
{
    point = getTransform().transformPoint(point);
}


////////////////////////////////////////////////////////////
void Transformable2d::inverseTransformPoint(Point2Df& point)
{
    point = getTransform().getInverse().transformPoint(point);
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
        // LOG_DEBUG << "[ " << sxc << " " << sys << " " << tx << " ]";
        // LOG_DEBUG << "[ " << -sxs << " " << syc << " " << ty << " ]";
        // LOG_DEBUG << "[ " << 0 << " " << 0 << " " << 1 << " ]";
        // clang-format on
        m_transformNeedUpdate = false;
    }

    return m_transform;
}


////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////


} // namespace transform
} // namespace geometry
} // namespace gfx
} // namespace he