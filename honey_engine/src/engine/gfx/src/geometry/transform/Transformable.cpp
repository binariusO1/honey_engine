#include "gfx/geometry/transform/Transformable.hpp"

#include "gfx/geometry/Utils.hpp"

namespace he
{
namespace gfx
{
namespace geometry
{
namespace transform
{
////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
Transformable<POINT, VECTOR>::Transformable() : m_scale(1.f)
{
}

////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
Transformable<POINT, VECTOR>::~Transformable() = default;


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
void Transformable<POINT, VECTOR>::move(const VECTOR& offset)
{
    setPosition(m_position + offset);
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
bool Transformable<POINT, VECTOR>::setPosition(const POINT& position)
{
    m_position = position;
    m_transformNeedUpdate = (m_position == position);
    return m_transformNeedUpdate;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
bool Transformable<POINT, VECTOR>::setOrigin(const POINT& origin)
{
    m_origin = origin;
    m_transformNeedUpdate = (m_origin == origin);
    return m_transformNeedUpdate;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
bool Transformable<POINT, VECTOR>::setScale(const VECTOR& factors)
{
    m_scale = factors;
    m_transformNeedUpdate = (m_scale == factors);
    return m_transformNeedUpdate;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
bool Transformable<POINT, VECTOR>::setRotation(const Angle& angle, const int)
{
    m_rotation[0] = angle.wrapUnsigned();
    m_transformNeedUpdate = (m_rotation[0] == angle.wrapUnsigned());
    return m_transformNeedUpdate;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
const POINT& Transformable<POINT, VECTOR>::getOrigin() const
{
    return m_origin;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
const Angle& Transformable<POINT, VECTOR>::getRotation(const int) const
{
    return m_rotation[0];
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
const VECTOR& Transformable<POINT, VECTOR>::getScale() const
{
    return m_scale;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
const POINT& Transformable<POINT, VECTOR>::getPosition() const
{
    return m_position;
}


////////////////////////////////////////////////////////////
//PROTECTED
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
void Transformable<POINT, VECTOR>::transformPoint(POINT& point)
{
    point = getTransform().transformPoint(point);
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
void Transformable<POINT, VECTOR>::inverseTransformPoint(POINT& point)
{
    point = getTransform().getInverse().transformPoint(point);
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
const Transform& Transformable<POINT, VECTOR>::getTransform() const
{
    if (m_transformNeedUpdate)
    {
        const auto positionMatrix = m_transform.getTranslateMatrix(m_position);
        const auto rotateXMatrix = m_transform.getRotateXMatrix(m_rotation[1]);
        const auto rotateYMatrix = m_transform.getRotateXMatrix(m_rotation[2]);
        const auto rotateZMatrix = m_transform.getRotateXMatrix(m_rotation[0]);
        const auto scaleMatrix = m_transform.getScaleMatrix(geometry::vectorToPoint(m_scale));
        const auto originMatrix = m_transform.getOriginMatrix(m_origin);

        m_transform = originMatrix*scaleMatrix*rotateXMatrix*rotateYMatrix*rotateZMatrix*positionMatrix;

        // note: jesli nie zadziała obrót wokół punktu środkowego to trzeba wtedy zaimplementować tx, ty dla origin poniżej:
        /*
        float angle  = -m_rotation[0].asRadians();
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
        */
        m_transformNeedUpdate = false;
    }

    return m_transform;
}

template class Transformable<geometry::Point2Df, geometry::Vector2Df>;
template class Transformable<geometry::Point3Df, geometry::Vector3Df>;

} // namespace transform
} // namespace geometry
} // namespace gfx
} // namespace he