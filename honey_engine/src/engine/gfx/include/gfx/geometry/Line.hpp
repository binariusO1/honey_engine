#pragma once

#include "gfx/geometry/Point2d.hpp"

namespace he
{
namespace gfx
{
namespace geometry
{
template<typename T>
struct Line
{
    geometry::Point2d<T> p1, p2;
};
} // namespace geometry
} // namespace gfx
} // namespace he