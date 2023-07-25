#pragma once

#include "geometry/figures/Rectangle.hpp"
#include "geometry/Line.hpp"

namespace gfx
{
namespace resources
{
struct Glyph
{
    float                              advance{0};  //!< Offset to move horizontally to the next character
    int                                lsbDelta{0}; //!< Left offset after forced autohint. Internally used by getKerning()
    int                                rsbDelta{0}; //!< Right offset after forced autohint. Internally used by getKerning()
    geometry::Line<float>           bounds;      //!< Bounding rectangle of the glyph, in coordinates relative to the baseline
    geometry::Line<float>           textureRect; //!< Texture coordinates of the glyph inside the font's texture
};
} // namespace resources
} // namespace gfx