#pragma once

#include "gfx/geometry/Angle.hpp"
#include "gfx/geometry/Point2d.hpp"
#include "gfx/geometry/Vector2d.hpp"

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

public:
    constexpr Transform(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22);
    constexpr const float* getMatrix() const;
    constexpr Transform getInverse() const;
    constexpr geometry::Point2Df transformPoint(const geometry::Point2Df& point) const;
    //constexpr FloatRect transformRect(const FloatRect& rectangle) const;

    constexpr Transform& combine(const Transform& transform);
    constexpr Transform& translate(const geometry::Vector2Df& offset);

    Transform& rotate(geometry::Angle angle);
    Transform& rotate(geometry::Angle angle, const geometry::Point2Df& center);
    constexpr Transform& scale(const geometry::Vector2Df& factors);
    constexpr Transform& scale(const geometry::Vector2Df& factors, const geometry::Point2Df& center);

    static const Transform Identity; //!< The identity transform (does nothing)

private:
    float m_matrix[16]{1.f, 0.f, 0.f, 0.f,
                       0.f, 1.f, 0.f, 0.f,
                       0.f, 0.f, 1.f, 0.f,
                       0.f, 0.f, 0.f, 1.f}; //!< 4x4 matrix defining the transformation

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