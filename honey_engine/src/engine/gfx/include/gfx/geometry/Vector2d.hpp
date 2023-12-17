#pragma once

#include <cstdint>
#include <iostream>

namespace he
{
namespace gfx
{
namespace geometry
{
template <typename T>
class Vector2d
{
public:
    inline Vector2d() : x{0}, y{0} {}
    inline Vector2d(T X, T Y) : x(X), y(Y) {}
    template <typename U>
    explicit Vector2d(const Vector2d<U>& vector) : x(static_cast<T>(vector.x)), y(static_cast<T>(vector.y)) {}
public:
    inline T getLength() {}
    
public:
    T x;
    T y;

friend std::ostream& operator<< (std::ostream& os, const geometry::Vector2d<T>& vector) {
    os << "{" << vector.x << ", " << vector.y << "}";
    return os;
}
};

template <typename T>
inline Vector2d<T> operator -(const Vector2d<T>& right)
{
    return Vector2d<T>(-right.x, -right.y);
}

template <typename T>
inline Vector2d<T>& operator +=(Vector2d<T>& left, const Vector2d<T>& right)
{
    left.x += right.x;
    left.y += right.y;

    return left;
}
template <typename T>
inline Vector2d<T>& operator -=(Vector2d<T>& left, const Vector2d<T>& right)
{
    left.x -= right.x;
    left.y -= right.y;

    return left;
}

template <typename T>
inline Vector2d<T> operator +(const Vector2d<T>& left, const Vector2d<T>& right)
{
    return Vector2d<T>(left.x + right.x, left.y + right.y);
}

template <typename T>
inline Vector2d<T> operator -(const Vector2d<T>& left, const Vector2d<T>& right)
{
    return Vector2d<T>(left.x - right.x, left.y - right.y);
}

template <typename T>
inline Vector2d<T> operator *(const Vector2d<T>& left, T right)
{
    return Vector2d<T>(left.x * right, left.y * right);
}

template <typename T>
inline Vector2d<T> operator *(T left, const Vector2d<T>& right)
{
    return Vector2d<T>(right.x * left, right.y * left);
}

template <typename T>
inline Vector2d<T>& operator *=(Vector2d<T>& left, T right)
{
    left.x *= right;
    left.y *= right;

    return left;
}

template <typename T>
inline Vector2d<T> operator /(const Vector2d<T>& left, T right)
{
    return Vector2d<T>(left.x / right, left.y / right);
}

template <typename T>
inline Vector2d<T>& operator /=(Vector2d<T>& left, T right)
{
    left.x /= right;
    left.y /= right;

    return left;
}

template <typename T>
inline bool operator ==(const Vector2d<T>& left, const Vector2d<T>& right)
{
    return (left.x == right.x) and (left.y == right.y);
}

template <typename T>
inline bool operator !=(const Vector2d<T>& left, const Vector2d<T>& right)
{
    return (left.x != right.x) or (left.y != right.y);
}

using Vector2D = Vector2d<std::int16_t>;
using Vector2Du = Vector2d<int>;
using Vector2Dui = Vector2d<unsigned int>;
using Vector2Df = Vector2d<float>;
using Vector2Dd = Vector2d<double>;

} // namespace geometry
} // namespace gfx
} // namespace he


