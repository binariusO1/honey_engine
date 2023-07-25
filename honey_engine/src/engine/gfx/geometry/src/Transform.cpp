#include "geometry/transform/Transform.hpp"

#include <cmath>

////////////////////////////////////////////////////////////


namespace geometry
{
namespace transform
{
////////////////////////////////////////////////////////////
Transform& Transform::rotate(geometry::Angle angle)
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


} // namespace transform
} // namespace geometry