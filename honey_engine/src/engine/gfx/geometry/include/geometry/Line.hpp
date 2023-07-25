#pragma once

#include "geometry/Point2d.hpp"

namespace geometry
{
template<typename T>
struct Line
{
    geometry::Point2d<T> p1, p2;
};
} // namespace geometry