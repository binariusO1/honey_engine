#pragma once

#include "gfx/resources/gfx/Color.hpp"

namespace geometry
{

template <typename T>
struct Vector4
{
    Vector4() = default;
    Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
    {
    }

    template <typename U>
    explicit Vector4(const Vector4<U>& other) :
    x(static_cast<T>(other.x)),
    y(static_cast<T>(other.y)),
    z(static_cast<T>(other.z)),
    w(static_cast<T>(other.w))
    {
    }

    Vector4(const gfx::Color& color)
    {
        copyVector(color, *this);
    }

    T x{0}; //!< 1st component (X) of the 4D vector
    T y{0}; //!< 2nd component (Y) of the 4D vector
    T z{0}; //!< 3rd component (Z) of the 4D vector
    T w{0}; //!< 4th component (W) of the 4D vector
};

using Vec4  = priv::Vector4<float>;
} // namespace geometry