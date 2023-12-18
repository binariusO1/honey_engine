#pragma once

#include "gfx/geometry/Point2d.hpp"
#include "gfx/geometry/Point3d.hpp"
#include "gfx/geometry/Vector2d.hpp"
#include "gfx/geometry/Vector3d.hpp"

namespace he
{
namespace gfx
{
namespace geometry
{
template <typename T>
inline Point2d<T> operator +(const Point2d<T>& point, const he::gfx::geometry::Vector2d<T>& vector)
{
    return Point2d<T>(point.x + vector.x, point.y + vector.y);
}

template <typename T>
inline Point3d<T> operator +(const Point3d<T>& point, const he::gfx::geometry::Vector3d<T>& vector)
{
    return Point3d<T>(point.x + vector.x, point.y + vector.y, point.z + vector.z);
}

template <typename T>
inline Point2d<T> vectorToPoint(const he::gfx::geometry::Vector2d<T>& vector)
{
    return Point2d<T>(vector.x, vector.y);
}

template <typename T>
inline Point3d<T> vectorToPoint(const he::gfx::geometry::Vector3d<T>& vector)
{
    return Point3d<T>(vector.x, vector.y, vector.z);
}

class Utils
{
public:
    static void perspective(float *matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar);
};

} // namespace he
} // namespace gfx
} // namespace geometry