#include "gfx/text/Page.hpp"

#include "gfx/Color.hpp"
#include "gfx/geometry/Size2d.hpp"
#include "gfx/graphic/Image.hpp"

namespace he
{
namespace gfx
{
namespace text
{
//////////////////////////////////////////////////////////////////////
Page::Page() : m_texture{std::make_shared<he::gfx::render::Texture>()}
{
}


//////////////////////////////////////////////////////////////////////
void Page::createTextureWithSmooth()
{
    createTexture();
    m_texture->setSmooth(true);
}


//////////////////////////////////////////////////////////////////////
void Page::createTextureWithoutSmooth()
{
    createTexture();
    m_texture->setSmooth(false);
}


//////////////////////////////////////////////////////////////////////
const std::shared_ptr<he::gfx::render::Texture> Page::getTexture() const
{
    return std::static_pointer_cast<he::gfx::render::Texture>(m_texture);
}


//////////////////////////////////////////////////////////////////////
const GlyphTable& Page::getGlyphTable() const
{
    return m_glyphTable;
}


//////////////////////////////////////////////////////////////////////
const std::vector<Row>& Page::getRows() const
{
    return m_rows;
}


//////////////////////////////////////////////////////////////////////
const unsigned int Page::getPositionOfNextRow() const
{
    return m_positionOfNextRow;
}


//////////////////////////////////////////////////////////////////////
void Page::addGlyph(const std::uint64_t key, const gfx::text::Glyph& glyph)
{
    m_glyphTable.emplace(key, glyph);
}


//////////////////////////////////////////////////////////////////////
void Page::addRowHeight(const unsigned int rowHeight)
{
    m_positionOfNextRow += rowHeight;
    m_rows.emplace_back(0, m_positionOfNextRow, rowHeight);
}


//////////////////////////////////////////////////////////////////////
bool Page::setWidthForRow(const size_t rowIndex, const unsigned int width)
{
    if (rowIndex < m_rows.size())
    {
        m_rows.at(rowIndex).width = width;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
void Page::createTexture()
{
    gfx::render::Image image({128, 128}, gfx::Color::White);

    // Note: Reserve a 2x2 white square for texturing underlines
    for (std::uint16_t i = 0; i < 2; ++i)
    {
        for (std::uint16_t j = 0; j < 2; ++j)
        {
            image.setPixel(geometry::Size2Dpxl{i, j}, gfx::Color::White);
        }
    }
    m_texture->create(image.getSize(), image.getData());
}
} // namespace text
} // namespace gfx
} // namespace he