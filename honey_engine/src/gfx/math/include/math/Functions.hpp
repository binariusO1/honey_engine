#pragma once

namespace math
{
template<typename T>
T getAbsoluteValue(const T& val)
{
    return ((val < 0) ? (val * (-1)) : val);
}

template <typename T>
T getMin(const T& a, const T& b)
{
    return (a < b) ? a : b;
}

template <typename T>
T getMax(const T& a, const T& b)
{
    return (a < b) ? b : a;
}
} // namespace math