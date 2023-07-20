#pragma once

namespace math
{
template<typename T>
T getAbsoluteValue(const T& val)
{
    return ((val < 0) ? (val * (-1)) : val);
}

} // namespace math