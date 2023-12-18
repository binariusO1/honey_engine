#pragma once

#include <cstdint>
#include <ostream>

namespace he
{
namespace gfx
{
namespace geometry
{
template <typename T>
class Point3d
{
public:
    constexpr Point3d() : x{0}, y{0}, z{0} {}
    constexpr Point3d(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    template <typename U>
    constexpr Point3d(const Point3d<U>& point) : x(static_cast<T>(point.x)), y(static_cast<T>(point.y), z(static_cast<T>(point.z))) {}

public:
    T x;
    T y;
    T z;

friend std::ostream& operator<< (std::ostream& os, const geometry::Point3d<T>& point) {
    os << "{" << point.x << ", " << point.y << ", " << point.z << "}";
    return os;
}

};

template <typename T>
inline Point3d<T> operator -(const Point3d<T>& right)
{
    return Point3d<T>(-right.x, -right.y, -right.z);
}

template <typename T>
inline Point3d<T>& operator +=(Point3d<T>& left, const Point3d<T>& right)
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;

    return left;
}
template <typename T>
inline Point3d<T>& operator -=(Point3d<T>& left, const Point3d<T>& right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;

    return left;
}

template <typename T>
inline Point3d<T> operator +(const Point3d<T>& left, const Point3d<T>& right)
{
    return Point3d<T>(left.x + right.x, left.y + right.y, left.z + right.z);
}

template <typename T>
inline Point3d<T> operator -(const Point3d<T>& left, const Point3d<T>& right)
{
    return Point3d<T>(left.x - right.x, left.y - right.y, left.z - right.z);
}

template <typename T>
inline Point3d<T> operator *(const Point3d<T>& left, T right)
{
    return Point3d<T>(left.x * right, left.y * right, left.z * right);
}

template <typename T>
inline Point3d<T> operator *(T left, const Point3d<T>& right)
{
    return Point3d<T>(right.x * left, right.y * left, right.z * left);
}

template <typename T>
inline Point3d<T>& operator *=(Point3d<T>& left, T right)
{
    left.x *= right;
    left.y *= right;
    left.z *= right;

    return left;
}

template <typename T>
inline Point3d<T> operator /(const Point3d<T>& left, T right)
{
    return Point3d<T>(left.x / right, left.y / right, left.z / right);
}

template <typename T>
inline Point3d<T>& operator /=(Point3d<T>& left, T right)
{
    left.x /= right;
    left.y /= right;
    left.z /= right;

    return left;
}

template <typename T>
inline bool operator ==(const Point3d<T>& left, const Point3d<T>& right)
{
    return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
}

template <typename T>
inline bool operator !=(const Point3d<T>& left, const Point3d<T>& right)
{
    return (left.x != right.x) || (left.y != right.y) || (left.z != right.z);
}

using Point3D = Point3d<std::int16_t>;
using Point3Di = Point3d<int>;
using Point3Du = Point3d<std::uint16_t>;
using Point3Dt = Point3d<std::uint64_t>;
using Point3Df = Point3d<float>;
using Point3Dd = Point3d<double>;

} // namespace geometry
} // namespace gfx
} // namespace he
