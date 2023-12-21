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
void Transformable<POINT, VECTOR>::move(const VECTOR& offset) // todo : bool
{
    setPosition(m_position + offset);
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
bool Transformable<POINT, VECTOR>::setPosition(const POINT& position)
{
    //  note: need always recalculate
    m_transformNeedUpdate = true;
    m_position = position;
    return m_transformNeedUpdate;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
bool Transformable<POINT, VECTOR>::setOrigin(const POINT& origin)
{
    //  note: need always recalculate
    m_transformNeedUpdate = true;
    m_origin = origin;
    return m_transformNeedUpdate;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
bool Transformable<POINT, VECTOR>::setScale(const VECTOR& factors)
{
    m_transformNeedUpdate = (m_scale != factors);
    m_scale = factors;
    return m_transformNeedUpdate;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
bool Transformable<POINT, VECTOR>::setRotation(const float rotationZ)
{
    const geometry::Angle angleZ(rotationZ);

    m_transformNeedUpdate = (m_rotations.z.wrapUnsigned() != angleZ.wrapUnsigned());

    m_rotations.axisOrder = AxisOrder::XYZ;
    m_rotations.x = geometry::Angle(0.f);
    m_rotations.y = geometry::Angle(0.f);
    m_rotations.z = angleZ;

    return m_transformNeedUpdate;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
bool Transformable<POINT, VECTOR>::setRotations(const float rotationX, const float rotationY, const float rotationZ, const AxisOrder axisOrder)
{
    const geometry::Angle angleX(rotationX);
    const geometry::Angle angleY(rotationY);
    const geometry::Angle angleZ(rotationZ);

    m_transformNeedUpdate = 
        (m_rotations.x.wrapUnsigned() != angleX.wrapUnsigned()) or 
        (m_rotations.y.wrapUnsigned() != angleY.wrapUnsigned()) or
        (m_rotations.z.wrapUnsigned() != angleZ.wrapUnsigned());
    
    m_rotations.axisOrder = axisOrder;
    m_rotations.x = angleX;
    m_rotations.y = angleY;
    m_rotations.z = angleZ;

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
const Angle& Transformable<POINT, VECTOR>::getRotation() const
{
    return m_rotations.z;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
const RotationArray& Transformable<POINT, VECTOR>::getRotations() const
{
    return m_rotations;
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
void Transformable<POINT, VECTOR>::inverseTransformPoint(geometry::Point2Df& point)
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
        const auto rotattionMatrix = getRotationMatrix();
        const auto scaleMatrix = m_transform.getScaleMatrix(geometry::vectorToPoint(m_scale));
        const auto originMatrix = m_transform.getOriginMatrix(m_origin);

        m_transform = positionMatrix*rotattionMatrix*scaleMatrix*originMatrix;
        // LOG_DEBUG << math::toString(m_transform.getMatrix()) << "X";
        // note: jesli nie zadziała obrót wokół punktu środkowego to trzeba wtedy zaimplementować tx, ty dla origin poniżej:
        /*
        float angle  = -m_rotations.x.asRadians();
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

////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
const Transform Transformable<POINT, VECTOR>::getRotationMatrix() const
{
    const auto rotateXMatrix = m_transform.getRotateXMatrix(m_rotations.x);
    const auto rotateYMatrix = m_transform.getRotateYMatrix(m_rotations.y);
    const auto rotateZMatrix = m_transform.getRotateZMatrix(m_rotations.z);

    switch (m_rotations.axisOrder)
    {
        case AxisOrder::XYZ:
            return rotateZMatrix * rotateYMatrix * rotateXMatrix;
        case AxisOrder::XZY:
            return rotateYMatrix * rotateZMatrix * rotateXMatrix;
        case AxisOrder::YZX:
            return rotateXMatrix * rotateZMatrix * rotateYMatrix;
        case AxisOrder::YXZ:
            return rotateZMatrix * rotateXMatrix * rotateYMatrix;
        case AxisOrder::ZYX:
            return rotateXMatrix * rotateYMatrix * rotateZMatrix;
        case AxisOrder::ZXY:
            return rotateYMatrix * rotateXMatrix * rotateZMatrix;
        default:
            return rotateZMatrix * rotateYMatrix * rotateXMatrix;
            break;
    }
}

template class Transformable<geometry::Point2Df, geometry::Vector2Df>;
template class Transformable<geometry::Point3Df, geometry::Vector3Df>;

} // namespace transform
} // namespace geometry
} // namespace gfx
} // namespace he