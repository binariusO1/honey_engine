#pragma once

#include <cstdint>
#include <ostream>

namespace geometry
{
template <typename T>
class Size2d
{
public:
    inline Size2d() : width{0}, height{0} {}
    inline Size2d(T A, T B) : width(A), height(B) {}
    inline bool isEmpty() const { return (width==0 and height==0) ? true : false; }
    template <typename U>
    explicit Size2d(const Size2d<U>& size) : width(static_cast<T>(size.width)), height(static_cast<T>(size.height)) {}

public:
    T width;
    T height;

    friend std::ostream& operator<< (std::ostream& os, const geometry::Size2d<T>& size)
    {
        os << "{" << size.width << ", " << size.height << "}";
        return os;
    }
};

template <typename T>
inline Size2d<T> operator -(const Size2d<T>& right)
{
    return Size2d<T>(-right.width, -right.height);
}

template <typename T>
inline Size2d<T>& operator +=(Size2d<T>& left, const Size2d<T>& right)
{
    left.width += right.width;
    left.height += right.height;

    return left;
}
template <typename T>
inline Size2d<T>& operator -=(Size2d<T>& left, const Size2d<T>& right)
{
    left.width -= right.width;
    left.height -= right.height;

    return left;
}

template <typename T>
inline Size2d<T> operator +(const Size2d<T>& left, const Size2d<T>& right)
{
    return Size2d<T>(left.width + right.width, left.height + right.height);
}

template <typename T>
inline Size2d<T> operator -(const Size2d<T>& left, const Size2d<T>& right)
{
    return Size2d<T>(left.width - right.width, left.height - right.height);
}

template <typename T>
inline Size2d<T> operator *(const Size2d<T>& left, T right)
{
    return Size2d<T>(left.width * right, left.height * right);
}

template <typename T>
inline Size2d<T> operator *(T left, const Size2d<T>& right)
{
    return Size2d<T>(right.width * left, right.height * left);
}

template <typename T>
inline Size2d<T>& operator *=(Size2d<T>& left, T right)
{
    left.width *= right;
    left.height *= right;

    return left;
}

template <typename T>
inline Size2d<T> operator /(const Size2d<T>& left, T right)
{
    return Size2d<T>(left.width / right, left.height / right);
}

template <typename T>
inline Size2d<T>& operator /=(Size2d<T>& left, T right)
{
    left.width /= right;
    left.height /= right;

    return left;
}

template <typename T>
inline bool operator ==(const Size2d<T>& left, const Size2d<T>& right)
{
    return (left.width == right.width) && (left.height == right.height);
}

template <typename T>
inline bool operator !=(const Size2d<T>& left, const Size2d<T>& right)
{
    return (left.width != right.width) || (left.height != right.height);
}

using Size2D    = Size2d<std::uint16_t>;
using Size2Dpxl = Size2d<std::uint16_t>;
using Size2Dui  = Size2d<std::uint32_t>;
using Size2Di   = Size2d<int>;
using Size2Df   = Size2d<float>;

} // namespace geometry


