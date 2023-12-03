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
class Point2d
{
public:
    constexpr Point2d() : x{0}, y{0} {}
    constexpr Point2d(T X, T Y) : x(X), y(Y) {}
    template <typename U>
    constexpr explicit Point2d(const Point2d<U>& point) : x(static_cast<T>(point.x)), y(static_cast<T>(point.y)) {}

public:
    T x;
    T y;

friend std::ostream& operator<< (std::ostream& os, const geometry::Point2d<T>& point) {
    os << "[" << point.x << ", " << point.y << "]";
    return os;
}  
};

template <typename T>
inline Point2d<T> operator -(const Point2d<T>& right)
{
    return Point2d<T>(-right.x, -right.y);
}

template <typename T>
inline Point2d<T>& operator +=(Point2d<T>& left, const Point2d<T>& right)
{
    left.x += right.x;
    left.y += right.y;

    return left;
}
template <typename T>
inline Point2d<T>& operator -=(Point2d<T>& left, const Point2d<T>& right)
{
    left.x -= right.x;
    left.y -= right.y;

    return left;
}

template <typename T>
inline Point2d<T> operator +(const Point2d<T>& left, const Point2d<T>& right)
{
    return Point2d<T>(left.x + right.x, left.y + right.y);
}

template <typename T>
inline Point2d<T> operator -(const Point2d<T>& left, const Point2d<T>& right)
{
    return Point2d<T>(left.x - right.x, left.y - right.y);
}

template <typename T>
inline Point2d<T> operator *(const Point2d<T>& left, T right)
{
    return Point2d<T>(left.x * right, left.y * right);
}

template <typename T>
inline Point2d<T> operator *(T left, const Point2d<T>& right)
{
    return Point2d<T>(right.x * left, right.y * left);
}

template <typename T>
inline Point2d<T>& operator *=(Point2d<T>& left, T right)
{
    left.x *= right;
    left.y *= right;

    return left;
}

template <typename T>
inline Point2d<T> operator /(const Point2d<T>& left, T right)
{
    return Point2d<T>(left.x / right, left.y / right);
}

template <typename T>
inline Point2d<T>& operator /=(Point2d<T>& left, T right)
{
    left.x /= right;
    left.y /= right;

    return left;
}

template <typename T>
inline bool operator ==(const Point2d<T>& left, const Point2d<T>& right)
{
    return (left.x == right.x) && (left.y == right.y);
}

template <typename T>
inline bool operator !=(const Point2d<T>& left, const Point2d<T>& right)
{
    return (left.x != right.x) || (left.y != right.y);
}

using Point2D = Point2d<std::int16_t>;
using Point2Di = Point2d<int>;
using Point2Du = Point2d<std::uint16_t>;
using Point2Dt = Point2d<std::uint64_t>;
using Point2Df = Point2d<float>;
using Point2Dd = Point2d<double>;

} // namespace geometry
} // namespace gfx
} // namespace he


