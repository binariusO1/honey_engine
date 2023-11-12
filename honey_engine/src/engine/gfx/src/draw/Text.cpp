#include "gfx/draw/Text.hpp"

#include <cmath>
#include "gfx/geometry/Line.hpp"
#include "gfx/graphic/Texture.hpp"
#include "gfx/render/Render.hpp"
#include "logger/Logger.hpp"

namespace
{
    // TODO copy from Shape.cpp
    // TODO make this function common not inside this .cpp file
void convertPixelPointToVertexPoint(he::gfx::geometry::Point2Df& point, const int screenWidth, const int screenHeight)
{
    const float SCREEN_SCALE = 2.0f;
    const float OFFSET_ORIGIN_SCREEN = -1.0f;

    if (screenWidth > 0 and screenHeight > 0)
    {
        float x = (point.x / static_cast<float>(screenWidth)*SCREEN_SCALE) + OFFSET_ORIGIN_SCREEN;
        float y = (point.y / static_cast<float>(screenHeight)*SCREEN_SCALE) + OFFSET_ORIGIN_SCREEN;
        point.x = x;
        point.y = y;
        return;
    }
    LOG_WARNING << "No convert. Window width or height is equal to 0!";
}
void convertPixelPointToVertexPointTexture(he::gfx::geometry::Point2Df& point, const he::gfx::geometry::Size2Dpxl& textureSize)
{
    const float SCREEN_SCALE = 1.f;
    const float OFFSET_ORIGIN_SCREEN = 0.0f;

    if (textureSize.width > 0 and textureSize.height > 0)
    {
        point.x *= (SCREEN_SCALE / textureSize.width);
        point.y *= (SCREEN_SCALE / textureSize.height);
        return;
    }
    LOG_WARNING << "No convert. Window width or height is equal to 0!";
}
} // namespace


namespace he::gfx::draw
{
//////////////////////////////////////////////////////////////////////
Text::Text(const std::string& name) : m_context(name) , m_font{std::make_shared<gfx::text::Font>()}
{
    m_characterSize = 30;
    updateVertexArray();
}


//////////////////////////////////////////////////////////////////////
Text::Text(const Text& copy)
    : m_string{copy.m_string}
    , m_font{copy.m_font ? std::make_shared<gfx::text::Font>(*copy.m_font) : nullptr}
    , m_letterSpacingFactor{copy.m_letterSpacingFactor}
    , m_lineSpacingFactor{copy.m_lineSpacingFactor}
    , m_context{copy.m_context}
    , m_outlineColor{copy.m_outlineColor}
    , m_outlineThickness{copy.m_outlineThickness}
    , m_characterSize{copy.m_characterSize}
    , m_style{copy.m_style}
    , m_bounds{copy.m_bounds}
    , m_vertexArray{copy.m_vertexArray}
    , m_outlineVertices{copy.m_outlineVertices}
    , m_fontTextureId{copy.m_fontTextureId}
    , m_originInCenter{copy.m_originInCenter}
    , Transformable2d{copy}
{
}


//////////////////////////////////////////////////////////////////////
Text::~Text() = default;


//////////////////////////////////////////////////////////////////////
void Text::setString(const std::string& string)
{
    m_string = string;
    updateVertexArray(); 
}


//////////////////////////////////////////////////////////////////////
const std::string& Text::getString() const
{
    return m_string;
}


//////////////////////////////////////////////////////////////////////
void Text::setFont(const text::Font& font)
{
    if (m_font.get() != &font)
    {
        m_font = std::make_shared<gfx::text::Font>(font);
        updateVertexArray();
    }
}


////////////////////////////////////////////////////////////
void Text::setFont(const std::filesystem::path& filepath)
{
    m_font = std::make_shared<gfx::text::Font>();
    m_font->loadFromFile(filepath);
    updateVertexArray();
}


////////////////////////////////////////////////////////////
void Text::setColor(const he::gfx::Color& color)
{
    if (color != m_context.color)
    {
        m_context.color = color;
    
        for (std::size_t i = 0; i < m_vertexArray.size(); ++i)
        {
            m_vertexArray[i].color = color;
        }
        updateVertexArray();
    }
}


////////////////////////////////////////////////////////////
he::gfx::Color Text::getColor() const
{
    return m_context.color;
}


////////////////////////////////////////////////////////////
unsigned int Text::getTextureId() const
{
    return m_font->getTexture(m_characterSize)->getTextureId();
}


////////////////////////////////////////////////////////////
const he::gfx::VertexArray2d& Text::getVertexArray() const
{
    return m_vertexArray;
}


////////////////////////////////////////////////////////////
void Text::setStyle(const std::uint32_t style)
{
    if (m_style != style)
    {
        m_style = style;
        updateVertexArray();
    }
}


////////////////////////////////////////////////////////////
const std::uint32_t Text::getStyle() const
{
    return m_style;
}


////////////////////////////////////////////////////////////
void Text::setOrigin(const geometry::Point2Df& point)
{
    Transformable2d::setOrigin(point);
}

////////////////////////////////////////////////////////////
void Text::setOriginInCenter()
{
    setOrigin({m_bounds.p2.x/2.f , m_bounds.p2.y/2.f});
}


////////////////////////////////////////////////////////////
void Text::setPosition(const he::gfx::geometry::Point2Df& point)
{ 
            LOG_DEBUG << LOG_BRIGHT_BLUE << point << LOG_WHITE;
    Transformable2d::setPosition(point);
    updateVertexArray();
};


////////////////////////////////////////////////////////////
const he::gfx::geometry::Point2Df& Text::getPosition() const
{ 
    return Transformable2d::getPosition();
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
void Text::updateVertexArray()
{
    if (not m_font or m_string.empty() or m_characterSize == 0)
    {
        return;
    }

    auto fontTextureId = m_font->getTexture(m_characterSize)->getTextureId();

    m_fontTextureId = fontTextureId;
    m_vertexArray.clear();
    m_outlineVertices.clear();
 
    computeTextStyle();

    for (std::size_t i = 0 ; i < m_vertexArray.size() ; ++i)
    {
        if (m_originInCenter)
        {
            setOriginInCenter();
        }

        auto point = m_vertexArray[i].position;
        auto texturePoint = m_vertexArray[i].texCoords;
        LOG_DEBUG << LOG_BRIGHT_BLUE << texturePoint << LOG_WHITE;
        transformPoint(point);
        // TODO: remove 1200, 800 - screen width and height
        convertPixelPointToVertexPoint(point, 1200, 800);
        convertPixelPointToVertexPointTexture(texturePoint, m_font->getTexture(m_characterSize)->getSize());
        m_vertexArray[i].position = point;
        m_vertexArray[i].texCoords = texturePoint;
    }
}


//////////////////////////////////////////////////////////////////////
void Text::computeTextStyle()
{
    LOG_DEBUG << LOG_BLUE << "computeTextStyle()" << LOG_WHITE;
    // Note: Compute values related to the text style
    bool  isBold             = m_style & Bold;
    bool  isUnderlined       = m_style & Underlined;
    bool  isStrikeThrough    = m_style & StrikeThrough;
    float italicShear        = (m_style & Italic) ? geometry::degrees(12).asRadians() : 0.f;
    float underlineOffset    = m_font->getUnderlinePosition(m_characterSize);
    float underlineThickness = m_font->getUnderlineThickness(m_characterSize);

    // Note:
    /*
        Compute the location of the strike through dynamically
        We use the center point of the lowercase 'x' glyph as the reference
        We reuse the underline thickness as the thickness of the strike through as well
    */
    geometry::Line<float> xBounds = m_font->getGlyph(U'x', m_characterSize, isBold).bounds;
    // was // float     strikeThroughOffset = xBounds.top + xBounds.height / 2.f;
    float     strikeThroughOffset = xBounds.p1.y + xBounds.p2.y / 2.f;

    // Precompute the variables needed by the algorithm
    float whitespaceWidth = m_font->getGlyph(U' ', m_characterSize, isBold).advance;
    float letterSpacing   = (whitespaceWidth / 3.f) * (m_letterSpacingFactor - 1.f);
    whitespaceWidth += letterSpacing;
    float lineSpacing = m_font->getLineSpacing(m_characterSize) * m_lineSpacingFactor;
    float x           = 0.f;
    auto  y           = static_cast<float>(m_characterSize);

    // Create one quad for each character
    auto          minX     = static_cast<float>(m_characterSize);
    auto          minY     = static_cast<float>(m_characterSize);
    float         maxX     = 0.f;
    float         maxY     = 0.f;

    std::uint32_t prevChar = 0;

    for (std::size_t i = 0; i < m_string.size(); ++i)
    {
        std::uint32_t curChar = m_string[i];

        // Skip the \r char to avoid weird graphical issues
        if (curChar == U'\r')
        {
            continue;
        }

        // Apply the kerning offset
        x += m_font->getKerning(prevChar, curChar, m_characterSize, isBold);

        // If we're using the underlined style and there's a new line, draw a line
        if (isUnderlined && (curChar == U'\n' && prevChar != U'\n'))
        {
            addLine(m_vertexArray, m_context.color, x, y, underlineOffset, underlineThickness);
            if (m_outlineThickness != 0)
            {
                addLine(m_outlineVertices, m_outlineColor, x, y, underlineOffset, underlineThickness, m_outlineThickness);
            }
        }

        // If we're using the strike through style and there's a new line, draw a line across all characters
        if (isStrikeThrough && (curChar == U'\n' && prevChar != U'\n'))
        {
            addLine(m_vertexArray, m_context.color, x, y, strikeThroughOffset, underlineThickness);
            if (m_outlineThickness != 0)
            {
                addLine(m_outlineVertices, m_outlineColor, x, y, strikeThroughOffset, underlineThickness, m_outlineThickness);
            }

        }

        prevChar = curChar;

        // Handle special characters
        if ((curChar == U' ') || (curChar == U'\n') || (curChar == U'\t'))
        {
            // Update the current bounds (min coordinates)
            minX = std::min(minX, x);
            minY = std::min(minY, y);

            switch (curChar)
            {
                case U' ':
                    x += whitespaceWidth;
                    break;
                case U'\t':
                    x += whitespaceWidth * 4;
                    break;
                case U'\n':
                    y += lineSpacing;
                    x = 0;
                    break;
            }

            // Update the current bounds (max coordinates)
            maxX = std::max(maxX, x);
            maxY = std::max(maxY, y);

            // Next glyph, no need to create a quad for whitespace
            continue;
        }

        // Apply the outline
        if (m_outlineThickness != 0)
        {
            const gfx::text::Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, isBold, m_outlineThickness);

            // Add the outline glyph to the vertices
            addGlyphQuad(m_outlineVertices, geometry::Point2Df(x, y), m_outlineColor, glyph, italicShear);
        }

        // Extract the current glyph's description
        const gfx::text::Glyph& glyph = m_font->getGlyph(curChar, m_characterSize, isBold);

        // Add the glyph to the vertices // 0 instead of y because we want bottom line equal to position y
        addGlyphQuad(m_vertexArray, geometry::Point2Df(x, 0), m_context.color, glyph, italicShear);

        // Update the current bounds
        float left   = glyph.bounds.p1.x;
        float top    = glyph.bounds.p1.y;
        float right  = glyph.bounds.p1.x + glyph.bounds.p2.x;
        float bottom = glyph.bounds.p1.y + glyph.bounds.p2.y;

        minX = std::min(minX, x + left - italicShear * bottom);
        maxX = std::max(maxX, x + right - italicShear * top);
        minY = std::min(minY, y + top);
        maxY = std::max(maxY, y + bottom);

        // Advance to the next character
        x += glyph.advance + letterSpacing;
    }
    // If we're using outline, update the current bounds
    if (m_outlineThickness != 0)
    {
        float outline = std::abs(std::ceil(m_outlineThickness));
        minX -= outline;
        maxX += outline;
        minY -= outline;
        maxY += outline;
    }

    // If we're using the underlined style, add the last line
    if (isUnderlined && (x > 0))
    {
        addLine(m_vertexArray, m_context.color, x, y, underlineOffset, underlineThickness);

        if (m_outlineThickness != 0)
        {
            addLine(m_outlineVertices, m_outlineColor, x, y, underlineOffset, underlineThickness, m_outlineThickness);
        }
    }
    // If we're using the strike through style, add the last line across all characters
    if (isStrikeThrough && (x > 0))
    {
        addLine(m_vertexArray, m_context.color, x, y, strikeThroughOffset, underlineThickness);

        if (m_outlineThickness != 0)
        {
            addLine(m_outlineVertices, m_outlineColor, x, y, strikeThroughOffset, underlineThickness, m_outlineThickness);
        }
    }
    // Update the bounding rectangle
    m_bounds.p1.x   = minX;
    m_bounds.p1.y    = minY;
    m_bounds.p2.x  = maxX - minX;
    m_bounds.p2.y = maxY - minY;

    return;
}


//////////////////////////////////////////////////////////////////////
void Text::draw(gfx::render::Render& render, const gfx::render::RenderSettings& renderSettings) const
{
            auto newRS = renderSettings;
            newRS.prymitiveType = he::libs::gl::ConnectionType::Triangles;
    if (m_font)
    {
        if (m_outlineThickness != 0.0)
        {
            render.drawVertex(m_outlineVertices, getTextureId(), m_context.color, newRS);
        }

        if (not getVertexArray().empty())
        {
            if (not m_geometryNeedUpdate)
            {
            LOG_DEBUG << LOG_BRIGHT_BLUE << "m_vertexArray.size(): " << m_vertexArray.size() << LOG_WHITE;
            for (size_t i = 0 ; i < m_vertexArray.size() ; ++i)
            {
                LOG_DEBUG << "vertex: [" << i << "], x: " << m_vertexArray[i].position.x << ", y: " << m_vertexArray[i].position.y <<
                " text x: " << m_vertexArray[i].texCoords.x << ", y: " << m_vertexArray[i].texCoords.y;
            }
            m_geometryNeedUpdate  = true;
            }

            render.drawVertex(m_vertexArray, getTextureId(), m_context.color, newRS);
        }
    }
}


//////////////////////////////////////////////////////////////////////
void Text::addLine(gfx::VertexArray2d& vertices, const gfx::Color& color,
             float            lineLength,
             float            lineTop,
             float            offset,
             float            thickness,
             float            outlineThickness)
{
    // Note: Add an underline or strikethrough line to the vertex array
    float top    = std::floor(lineTop + offset - (thickness / 2) + 0.5f);
    float bottom = top + std::floor(thickness + 0.5f);
    const geometry::Point2Df texturePoint{1.0, 1.0};

    vertices.push_back(gfx::Vertex2d(geometry::Point2Df(-outlineThickness, top - outlineThickness),                color, texturePoint));
    vertices.push_back(gfx::Vertex2d(geometry::Point2Df(lineLength + outlineThickness, top - outlineThickness),    color, texturePoint));
    vertices.push_back(gfx::Vertex2d(geometry::Point2Df(-outlineThickness, bottom + outlineThickness),             color, texturePoint));
    vertices.push_back(gfx::Vertex2d(geometry::Point2Df(-outlineThickness, bottom + outlineThickness),             color, texturePoint));
    vertices.push_back(gfx::Vertex2d(geometry::Point2Df(lineLength + outlineThickness, top - outlineThickness),    color, texturePoint));
    vertices.push_back(gfx::Vertex2d(geometry::Point2Df(lineLength + outlineThickness, bottom + outlineThickness), color, texturePoint));
}


void Text::addGlyphQuad(gfx::VertexArray2d& vertices, const geometry::Point2Df& position, const gfx::Color& color, const gfx::text::Glyph& glyph, float italicShear)
{

    LOG_DEBUG << "addGlyphQuad()" << LOG_WHITE << "position: " << position << "glyph: "; 
    LOG_DEBUG << "glyph.bounds.p1.x: " << glyph.bounds.p1.x
    << ", glyph.bounds.p1.y: " << glyph.bounds.p1.y
    << ", glyph.bounds.p2.x: " << glyph.bounds.p2.x
    << ", glyph.bounds.p2.y: " << glyph.bounds.p2.y
    << " , glyph.textureRect.p1.x: " << glyph.textureRect.p1.x
    << ", glyph.textureRect.p1.y: " << glyph.textureRect.p1.y
    << ", glyph.textureRect.p2.x: " << glyph.textureRect.p2.x
    << ", glyph.textureRect.p2.y: " << glyph.textureRect.p2.y;

    float padding = 1.0;

    float left   = glyph.bounds.p1.x - padding;
    float top    = glyph.bounds.p1.y - padding;
    float right  = glyph.bounds.p1.x + glyph.bounds.p2.x + padding;
    float bottom = glyph.bounds.p1.y + glyph.bounds.p2.y + padding;

    float u1 = static_cast<float>(glyph.textureRect.p1.x) - padding;
    float v1 = static_cast<float>(glyph.textureRect.p1.y) - padding;
    float u2 = static_cast<float>(glyph.textureRect.p1.x + glyph.textureRect.p2.x) + padding;
    float v2 = static_cast<float>(glyph.textureRect.p1.y + glyph.textureRect.p2.y) + padding;

    vertices.push_back(gfx::Vertex2d(geometry::Point2Df(position.x + left - italicShear * top, position.y - top),          color, geometry::Point2Df(u1, v1))); //1
    vertices.push_back(gfx::Vertex2d(geometry::Point2Df(position.x + right - italicShear * top, position.y - top),         color, geometry::Point2Df(u2, v1))); //2
    vertices.push_back(gfx::Vertex2d(geometry::Point2Df(position.x + left - italicShear * bottom, position.y - bottom),    color, geometry::Point2Df(u1, v2))); //3
    vertices.push_back(gfx::Vertex2d(geometry::Point2Df(position.x + left - italicShear * bottom, position.y - bottom),    color, geometry::Point2Df(u1, v2))); //4
    vertices.push_back(gfx::Vertex2d(geometry::Point2Df(position.x + right - italicShear * top, position.y - top),         color, geometry::Point2Df(u2, v1))); //5
    vertices.push_back(gfx::Vertex2d(geometry::Point2Df(position.x + right - italicShear * bottom, position.y - bottom),   color, geometry::Point2Df(u2, v2))); //6
}
} // namespace he::gfx::draw