#pragma once

#include <cstdint>
#include "Color.hpp"

namespace gfx
{
struct ColorBasic
{
    constexpr ColorBasic();
    constexpr ColorBasic(std::uint8_t red, std::uint8_t green, std::uint8_t blue);
    constexpr ColorBasic(const gfx::Color sample);
    constexpr explicit ColorBasic(std::uint32_t color);
    constexpr std::uint32_t toInteger() const;

    std::uint8_t r{0};   //!< Red component
    std::uint8_t g{0};   //!< Green component
    std::uint8_t b{0};   //!< Blue component
};

[[nodiscard]] constexpr bool operator==(const ColorBasic& left, const ColorBasic& right);
[[nodiscard]] constexpr bool operator!=(const ColorBasic& left, const ColorBasic& right);
[[nodiscard]] constexpr ColorBasic operator+(const ColorBasic& left, const ColorBasic& right);
[[nodiscard]] constexpr ColorBasic operator-(const ColorBasic& left, const ColorBasic& right);
[[nodiscard]] constexpr ColorBasic operator*(const ColorBasic& left, const ColorBasic& right);
constexpr ColorBasic& operator+=(ColorBasic& left, const ColorBasic& right);
constexpr ColorBasic& operator-=(ColorBasic& left, const ColorBasic& right);
constexpr ColorBasic& operator*=(ColorBasic& left, const ColorBasic& right);

#include "ColorBasic.inl"

} // namespace gfx