#pragma once

#include <cstdint>
#include <string>
#include "logger/Logger.hpp"

namespace he
{
namespace math
{

using Matrix4x4 = float[16];

inline void transformPoint2d(float& x, float& y, const Matrix4x4 matrix)
{
    if (matrix)
    {
            x = matrix[0] * x + matrix[4] * y + matrix[12];
            y = matrix[1] * x + matrix[5] * y + matrix[13];
    }
}

inline void setDiagonal(const float diagonalValue, Matrix4x4 matrix)
{
    if (matrix)
    {
        for (std::size_t i = 0 ; i < 4 ; ++i)
        {
            for (std::size_t j = 0 ; j < 4 ; ++j)
            {
                if (i == j)
                {
                    matrix[i+j] = diagonalValue;
                }
            }
        }
    }
}

inline const std::string toString(const Matrix4x4 matrix)
{
    std::string stringMatrix{};

    if (matrix)
    {
        for (std::size_t i = 0 ; i < 16 ; i=i+4)
        {
            for (std::size_t j = 0 ; j < 4 ; ++j)
            {
                stringMatrix+=std::to_string(matrix[i+j]);
                stringMatrix+=' ';
            }
            stringMatrix+='\n';
        }
    }
    return stringMatrix;
}

/*
template <std::size_t Columns, std::size_t Rows>
struct Matrix
{
    explicit Matrix(float mainDiagonal)
    {
        for (std::size_t i = 0 ; i < Columns ; ++i)
        {
            for (std::size_t j = 0 ; j < Rows ; ++j)
            {
                if (i == j)
                {
                    array[i+j] = mainDiagonal;
                }
            }
        }
    }

    float* getMatrix()
    {
        return array;
    }

    float array[Columns * Rows];
};

using Matrix3x3 = Matrix<3, 3>;
using Matrix4x4 = Matrix<4, 4>;
*/
} // namespace math
} // namespace he