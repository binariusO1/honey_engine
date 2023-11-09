#pragma once

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
using GlyphTable = std::unordered_map<std::uint64_t, gfx::text::Glyph>; // Note: Table mapping a codepoint to its glyph

struct Row
{
    unsigned int width{0};      // Note: Current width of the row
    unsigned int top{0};        // Note: Y position of the row into the texture
    unsigned int height{0};     // Note: Height of the row
};


class Page
{
public:
    Page();

public:
    void createTextureWithSmooth();
    void createTextureWithoutSmooth();
    const std::shared_ptr<he::gfx::render::Texture> getTexture() const;
    const GlyphTable& getGlyphTable() const;
    const std::vector<Row>& getRows() const;
    const unsigned int getPositionOfNextRow() const;

    void addGlyph(const std::uint64_t key, const gfx::text::Glyph& glyph);
    void addRowHeight(const unsigned int rowHeight);
    bool setWidthForRow(const size_t rowIndex, const unsigned int width);

private:
    void createTexture();

protected:
    std::shared_ptr<he::gfx::render::ITexture> m_texture{nullptr}; // Note: Texture containing the pixels of the glyphs

private:
    GlyphTable m_glyphTable;            // Note: Table mapping code points to their corresponding glyph
    std::vector<Row> m_rows;            // Note: List containing the position of all the existing rows
    unsigned int m_positionOfNextRow{3};   // Note: position of the next new row in the texture

};
} // namespace text
} // namespace gfx
} // namespace he