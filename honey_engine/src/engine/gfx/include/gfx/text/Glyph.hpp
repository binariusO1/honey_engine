#pragma once

#include "gfx/geometry/Line.hpp"

namespace he
{
namespace gfx
{
namespace text
{
struct Glyph
{
    float advance{0};                   //!< Offset to move horizontally to the next character
    int lsbDelta{0};                    //!< Left offset after forced autohint. Internally used by getKerning()
    int rsbDelta{0};                    //!< Right offset after forced autohint. Internally used by getKerning()
    geometry::Line<float> bounds;       //!< Bounding rectangle of the glyph, in coordinates relative to the baseline
    geometry::Line<float> textureRect;  //!< Texture coordinates of the glyph inside the font's texture
};
} // namespace text
} // namespace gfx
} // namespace he