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
class Vector3d
{
public:
    inline Vector3d() : x{0}, y{0}, z{0} {}
    inline Vector3d(T value) : x(value), y(value), z(value) {}
    inline Vector3d(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
    template <typename U>
    explicit Vector3d(const Vector3d<U>& vector) : x(static_cast<T>(vector.x)), y(static_cast<T>(vector.y)), z(static_cast<T>(vector.z)) {}
public:
    inline std::size_t size() { return 3; }
    
public:
    T x;
    T y;
    T z;

friend std::ostream& operator<< (std::ostream& os, const geometry::Vector3d<T>& vector)
{
    os << "{" << vector.x << ", " << vector.y < ", " << vector.z << "}";
    return os;
}
};

template <typename T>
inline Vector3d<T> operator -(const Vector3d<T>& right)
{
    return Vector3d<T>(-right.x, -right.y, -right.z);
}

template <typename T>
inline Vector3d<T>& operator +=(Vector3d<T>& left, const Vector3d<T>& right)
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;

    return left;
}
template <typename T>
inline Vector3d<T>& operator -=(Vector3d<T>& left, const Vector3d<T>& right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;

    return left;
}

template <typename T>
inline Vector3d<T> operator +(const Vector3d<T>& left, const Vector3d<T>& right)
{
    return Vector3d<T>(left.x + right.x, left.y + right.y, left.z + right.z);
}

template <typename T>
inline Vector3d<T> operator -(const Vector3d<T>& left, const Vector3d<T>& right)
{
    return Vector3d<T>(left.x - right.x, left.y - right.y, left.z - right.z);
}

template <typename T>
inline Vector3d<T> operator *(const Vector3d<T>& left, T right)
{
    return Vector3d<T>(left.x * right, left.y * right, left.z * right);
}

template <typename T>
inline Vector3d<T> operator *(T left, const Vector3d<T>& right)
{
    return Vector3d<T>(right.x * left, right.y * left, right.z * left);
}

template <typename T>
inline Vector3d<T>& operator *=(Vector3d<T>& left, T right)
{
    left.x *= right;
    left.y *= right;
    left.z *= right;

    return left;
}

template <typename T>
inline Vector3d<T> operator /(const Vector3d<T>& left, T right)
{
    return Vector3d<T>(left.x / right, left.y / right, left.z / right);
}

template <typename T>
inline Vector3d<T>& operator /=(Vector3d<T>& left, T right)
{
    left.x /= right;
    left.y /= right;
    left.z /= right;

    return left;
}

template <typename T>
inline bool operator ==(const Vector3d<T>& left, const Vector3d<T>& right)
{
    return (left.x == right.x) and (left.y == right.y) and (left.z == right.z);
}

template <typename T>
inline bool operator !=(const Vector3d<T>& left, const Vector3d<T>& right)
{
    return (left.x != right.x) or (left.y != right.y) or (left.z != right.z);
}

using Vector3D = Vector3d<std::int16_t>;
using Vector3Du = Vector3d<int>;
using Vector3Dui = Vector3d<unsigned int>;
using Vector3Df = Vector3d<float>;
using Vector3Dd = Vector3d<double>;

} // namespace geometry
} // namespace gfx
} // namespace he


