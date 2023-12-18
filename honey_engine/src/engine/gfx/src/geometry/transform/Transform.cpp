#include "gfx/geometry/transform/Transform.hpp"

#include <cmath>

////////////////////////////////////////////////////////////

namespace he
{
namespace gfx
{
namespace geometry
{
namespace transform
{
////////////////////////////////////////////////////////////
Transform& Transform::rotateAroundX(geometry::Angle angle)
{
    float rad = angle.asRadians();
    float cos = std::cos(rad);
    float sin = std::sin(rad);

    // clang-format off
    Transform rotation(1, 0, 0,
                       0, cos, -sin,
                       0, sin, cos);
    // clang-format on

    return combine(rotation);
}


////////////////////////////////////////////////////////////
Transform& Transform::rotateAroundY(geometry::Angle angle)
{
    float rad = angle.asRadians();
    float cos = std::cos(rad);
    float sin = std::sin(rad);

    // clang-format off
    Transform rotation(cos, 0, sin,
                       0, 1, 0,
                       -sin, 0, cos);
    // clang-format on

    return combine(rotation);
}


////////////////////////////////////////////////////////////
Transform& Transform::rotateAroundZ(geometry::Angle angle)
{
    float rad = angle.asRadians();
    float cos = std::cos(rad);
    float sin = std::sin(rad);

    // clang-format off
    Transform rotation(cos, -sin, 0,
                       sin,  cos, 0,
                       0,    0,   1);
    // clang-format on

    return combine(rotation);
}


////////////////////////////////////////////////////////////
Transform& Transform::rotate(geometry::Angle angle, const geometry::Point2Df& center)
{
    float rad = angle.asRadians();
    float cos = std::cos(rad);
    float sin = std::sin(rad);

    // clang-format off
    Transform rotation(cos, -sin, center.x * (1 - cos) + center.y * sin,
                       sin,  cos, center.y * (1 - cos) - center.x * sin,
                       0,    0,   1);
    // clang-format on

    return combine(rotation);
}


////////////////////////////////////////////////////////////
Transform& Transform::perspective(const float fovyInDegrees, const float aspectRatio, const float zNear, const float zFar)
{
    float const a = 1.f / tanf(fovyInDegrees / 2.f);

    Transform transform(a/aspectRatio,       0.f,                                                0.f,  0.f,
                        0.f,                   a,                                                0.f,  0.f,
                        0.f,                 0.f,                 -((zFar + zNear) / (zFar - zNear)), -1.f,
                        0.f,                 0.f,  -((2.f * fovyInDegrees * zNear) / (zFar - zNear)),  0.f);

    return combine(transform);
}

} // namespace transform
} // namespace geometry
} // namespace gfx
} // namespace he