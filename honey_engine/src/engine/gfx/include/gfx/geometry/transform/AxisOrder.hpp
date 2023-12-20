#pragma once

#include <cstdint>

namespace he
{
namespace gfx
{
namespace geometry
{
namespace transform
{
enum class AxisOrder : std::uint8_t
{
    XYZ = 0,
    XZY,
    YZX,
    YXZ,
    ZYX,
    ZXY
};
} // namespace transf
} // namespace geometry
} // namespace gfx
} // namespace he