#include "gfx/text/Page.hpp"

#include "exception/invalid_initialization.hpp"
#include "gfx/Color.hpp"
#include "gfx/geometry/Size2d.hpp"
#include "gfx/graphic/Image.hpp"
#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace text
{
//////////////////////////////////////////////////////////////////////
Page::Page(bool smooth) : m_nextRow(3)
{
    gfx::render::Image image({128, 128}, gfx::Color::White);

    // Note: Reserve a 2x2 white square for texturing underlines
    for (std::size_t i = 0; i < 2; ++i)
    {
        for (std::size_t j = 0; j < 2; ++j)
        {
            image.setPixel(geometry::Size2Dpxl{static_cast<uint16_t>(i), static_cast<uint16_t>(j)}, gfx::Color(255, 255, 255, 255));
        }
    }

    try
    {
        m_texture = std::make_shared<he::gfx::render::Texture>(image);
    }
    catch(const std::exception& e)
    {
        throw he::common::invalid_initialization("Failed to load font page texture");
    }

    m_texture->setSmooth(smooth);
}


//////////////////////////////////////////////////////////////////////
const std::shared_ptr<he::gfx::render::Texture> Page::getTexture() const
{
    return m_texture;
}


//////////////////////////////////////////////////////////////////////
const GlyphTable& Page::getGlyphTable() const
{
    return m_glyphs;
}


//////////////////////////////////////////////////////////////////////
void Page::addGlyph(const std::uint64_t key, const gfx::text::Glyph& glyph)
{
    m_glyphs.emplace(key, glyph);
}


//////////////////////////////////////////////////////////////////////
std::vector<Row>& Page::getRows()
{
    return m_rows;
}


//////////////////////////////////////////////////////////////////////
void Page::addRow(const unsigned int rowHeight)
{
    m_rows.emplace_back(getNextRow(), rowHeight);
}


//////////////////////////////////////////////////////////////////////
const unsigned int Page::getNextRow() const
{
    return m_nextRow;
}


//////////////////////////////////////////////////////////////////////
void Page::setRow(const unsigned int row)
{
    m_nextRow = row;
}
} // namespace text
} // namespace gfx
} // namespace he