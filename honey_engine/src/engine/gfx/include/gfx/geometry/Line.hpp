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
    // Line(geometry::Point2d<T> p1, geometry::Point2d<T> p2) : p1(p1) , p2(p2) {}
    geometry::Point2d<T> p1, p2;
};
} // namespace geometry
} // namespace gfx
} // namespace he