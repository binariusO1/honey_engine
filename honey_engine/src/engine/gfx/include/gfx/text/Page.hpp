#pragma once

#include <map>
#include <unordered_map>
#include <vector>

#include "gfx/graphic/Texture.hpp"
#include "gfx/text/Glyph.hpp"

namespace he
{
namespace gfx
{
namespace text
{
struct Row
{
    Row() = default;
    Row(unsigned int rowTop, unsigned int rowHeight) : top(rowTop), height(rowHeight)
    {
    }

    unsigned int width{0};      //!< Current width of the row
    unsigned int top{0};        //!< Y position of the row into the texture
    unsigned int height{0};     //!< Height of the row
};

using GlyphTable = std::unordered_map<std::uint64_t, gfx::text::Glyph>; //!< Table mapping a codepoint to its glyph

class Page
{
public:
    Page() = delete;
    explicit Page(bool smooth);

public:
    const std::shared_ptr<he::gfx::render::Texture> getTexture() const;
    const GlyphTable& getGlyphTable() const;
    void addGlyph(const std::uint64_t key, const gfx::text::Glyph& glyph);
    std::vector<Row>& getRows();
    void addRow(const unsigned int rowHeight);
    const unsigned int getNextRow() const;
    void setRow(const unsigned int);

private:
    GlyphTable m_glyphs;  //!< Table mapping code points to their corresponding glyph
    std::shared_ptr<he::gfx::render::Texture> m_texture{nullptr}; //!< Texture containing the pixels of the glyphs
    unsigned int m_nextRow; //!< Y position of the next new row in the texture
    std::vector<Row> m_rows;    //!< List containing the position of all the existing rows
};
} // namespace text
} // namespace gfx
} // namespace he