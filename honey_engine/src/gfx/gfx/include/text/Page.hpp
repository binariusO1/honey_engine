#pragma once

#include <map>
#include <unordered_map>
#include <vector>

#include "graphics/Texture.hpp"
#include "text/Glyph.hpp"

namespace gfx
{
namespace resources
{
struct Row
{
    Row(unsigned int rowTop, unsigned int rowHeight) : top(rowTop), height(rowHeight)
    {
    }

    unsigned int width{0}; //!< Current width of the row
    unsigned int top;      //!< Y position of the row into the texture
    unsigned int height;   //!< Height of the row
};

using GlyphTable = std::unordered_map<std::uint64_t, gfx::resources::Glyph>; //!< Table mapping a codepoint to its glyph

struct Page
{
    explicit Page(bool smooth);

    GlyphTable                  glyphs;  //!< Table mapping code points to their corresponding glyph
    gfx::resources::Texture          texture; //!< Texture containing the pixels of the glyphs
    unsigned int                nextRow; //!< Y position of the next new row in the texture
    std::vector<Row>            rows;    //!< List containing the position of all the existing rows
};
} // namespace resources
} // namespace gfx