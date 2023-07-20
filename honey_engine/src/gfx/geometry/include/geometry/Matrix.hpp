#pragma once

namespace geometry
{
template <std::size_t Columns, std::size_t Rows>
struct Matrix
{
    explicit Matrix(const float* pointer)
    {
        copyMatrix(pointer, Columns * Rows, array);
    }

    Matrix(const Transform& transform)
    {
        copyMatrix(transform, *this);
    }

    float array[Columns * Rows];
};

using Mat3  = priv::Matrix<3, 3>;
using Mat4  = priv::Matrix<4, 4>;
} // namespace geometry