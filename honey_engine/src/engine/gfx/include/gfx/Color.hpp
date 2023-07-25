#pragma once

#include <cstdint>
#include <ostream>

namespace he
{
namespace gfx
{
struct Color
{
    constexpr Color();
    constexpr Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha);
    constexpr explicit Color(std::uint32_t color);
    constexpr std::uint32_t toInteger() const;

    static const Color Black;       //!< Black predefined color
    static const Color White;       //!< White predefined color
    static const Color Red;         //!< Red predefined color
    static const Color Green;       //!< Green predefined color
    static const Color Blue;        //!< Blue predefined color
    static const Color Yellow;      //!< Yellow predefined color
    static const Color Magenta;     //!< Magenta predefined color
    static const Color Cyan;        //!< Cyan predefined color
    static const Color Transparent; //!< Transparent (black) predefined color

    std::uint8_t r{0};   //!< Red component
    std::uint8_t g{0};   //!< Green component
    std::uint8_t b{0};   //!< Blue component
    std::uint8_t a{0};   //!< Alpha component

    friend std::ostream& operator<< (std::ostream& os, const gfx::Color& color)
    {
        os << "{" << static_cast<int>(color.r) << ", " << static_cast<int>(color.g) << ", " << static_cast<int>(color.b) << ", " << static_cast<int>(color.a) << "}";
        return os;
    }
};

[[nodiscard]] constexpr bool operator==(const Color& left, const Color& right);
[[nodiscard]] constexpr bool operator!=(const Color& left, const Color& right);
[[nodiscard]] constexpr Color operator+(const Color& left, const Color& right);
[[nodiscard]] constexpr Color operator-(const Color& left, const Color& right);
[[nodiscard]] constexpr Color operator*(const Color& left, const Color& right);
constexpr Color& operator+=(Color& left, const Color& right);
constexpr Color& operator-=(Color& left, const Color& right);
constexpr Color& operator*=(Color& left, const Color& right);

#include "Color.inl"

} // namespace gfx
} // namespace he