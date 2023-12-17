#pragma once

#include "gfx/geometry/Angle.hpp"
#include "gfx/geometry/Point2d.hpp"
#include "gfx/geometry/Vector2d.hpp"
#include "gfx/geometry/Vector3d.hpp"
#include "math/Matrix.hpp"

namespace he
{
namespace gfx
{
namespace geometry
{
namespace transform
{

class Transform
{
public:
    constexpr Transform();
    constexpr Transform(const float diagonalValue);
    constexpr Transform(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22);
    constexpr Transform(float a00, float a01, float a02, float a03, float a10, float a11, float a12, float a13, float a20, float a21, float a22, float a23, float a30, float a31, float a32, float a33);

public:
    constexpr const float* getMatrix() const;
    constexpr Transform getInverse() const;
    constexpr geometry::Point2Df transformPoint(const geometry::Point2Df& point) const;

    constexpr Transform& combine(const Transform& transform);
    constexpr Transform& translate(const geometry::Vector2Df& offset);
    constexpr Transform& translate(const geometry::Vector3Df& offset);

    Transform& rotateAroundX(geometry::Angle angle);
    Transform& rotateAroundY(geometry::Angle angle);
    Transform& rotateAroundZ(geometry::Angle angle);
    Transform& rotate(geometry::Angle angle, const geometry::Point2Df& center);
    constexpr Transform& scale(const geometry::Vector2Df& factors);
    constexpr Transform& scale(const geometry::Vector2Df& factors, const geometry::Point2Df& center);

    Transform& perspective(const float fovyInDegrees, const float aspectRatio, const float zNear, const float zFar);

    static const Transform Identity; //!< The identity transform (does nothing)

private:
    math::Matrix4x4 m_matrix{0.f, 0.f, 0.f, 0.f,
                             0.f, 0.f, 0.f, 0.f,
                             0.f, 0.f, 0.f, 0.f,
                             0.f, 0.f, 0.f, 0.f}; //!< 4x4 matrix defining the transformation

};

constexpr Transform operator*(const Transform& left, const Transform& right);
constexpr Transform& operator*=(Transform& left, const Transform& right);
constexpr geometry::Point2Df operator*(const Transform& left, const geometry::Point2Df& right);
constexpr bool operator==(const Transform& left, const Transform& right);
constexpr bool operator!=(const Transform& left, const Transform& right);

#include "gfx/geometry/transform/Transform.inl"

} // namespace transform
} // namespace geometry
} // namespace gfx
} // namespace he