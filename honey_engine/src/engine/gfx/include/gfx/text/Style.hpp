#pragma once

#include <cstdint>

namespace he
{
namespace gfx
{
namespace text
{
typedef std::uint32_t Style;

enum FontStyle : std::uint32_t
{
    Regular       = 0,      //!< Regular characters, no style
    Bold          = 1 << 0, //!< Bold characters
    Italic        = 1 << 1, //!< Italic characters
    Underlined    = 1 << 2, //!< Underlined characters
    StrikeThrough = 1 << 3  //!< Strike through characters
};
} // namespace text
} // namespace gfx
} // namespace he