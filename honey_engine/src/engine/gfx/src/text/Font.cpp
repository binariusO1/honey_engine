#include "gfx/text/Font.hpp"

#include <cmath>
#include <optional>
#include "gfx/geometry/Line.hpp"
#include "gfx/geometry/Size2d.hpp"
#include "gfx/graphic/Texture.hpp"
#include "gfx/text/Glyph.hpp"
#include "gfx/text/FontHandles.hpp"
#include "gfx/text/FreeType2Wrapper.hpp"
#include "logger/Logger.hpp"

namespace
{
const char* projectPath = "D:\\Projects\\CPP\\games\\honey_engine\\";
const char* loadFontErr = "Unable to load font!";

//////////////////////////////////////////////////////////////////////
std::string getAbsolutePath(const std::filesystem::path& filepath)
{
    std::string path = filepath.string().c_str();
    return projectPath + path;
}


//////////////////////////////////////////////////////////////////////
template <typename T, typename U>
inline T reinterpret(const U& input)
{
    T output;
    memcpy(&output, &input, sizeof(U));
    return output;
}


//////////////////////////////////////////////////////////////////////
std::uint64_t combine(float outlineThickness, bool bold, std::uint32_t index)
{
    // Note: Combine outline thickness, boldness and font glyph index into a single 64-bit key
    return (static_cast<std::uint64_t>(reinterpret<std::uint32_t>(outlineThickness)) << 32) |
           (static_cast<std::uint64_t>(bold) << 31) | index;
}


} // namespace


namespace he::gfx::text
{
//////////////////////////////////////////////////////////////////////
Font::Font() : m_freeType2Wrapper{std::make_shared<gfx::text::FreeType2Wrapper>()}
{
}


////////////////////////////////////////////////////////////
Font::Font(const Font& copy) 
    : m_fontHandles(copy.m_fontHandles)
    , m_isSmooth(copy.m_isSmooth)
    , m_info(copy.m_info)
    , m_pages(copy.m_pages)
    , m_pixelBuffer(copy.m_pixelBuffer)
    , m_freeType2Wrapper{copy.m_freeType2Wrapper}
{
}

////////////////////////////////////////////////////////////
bool Font::loadFromFile(const std::filesystem::path& filepath)
{
    if (checkFilePathString(filepath))
    {
        const std::string& absolutePath = getAbsolutePath(filepath);

        if (initializeFreeType(absolutePath))
        {
            return true;
        }
    }

    LOG_ERROR << "Failed to create the font. Error code: " << m_freeType2Wrapper->getErrorDesc();
    return false;
}


////////////////////////////////////////////////////////////
bool Font::setCharacterSize(const unsigned int characterSize)
{
    if (trySetCharacterSize(characterSize))
    {   
        return true;
    }

    LOG_WARNING << "Failed to set font size to: " << characterSize;
    return false;
}


////////////////////////////////////////////////////////////
const std::shared_ptr<he::gfx::render::Texture> Font::getTexture() const
{
    return loadPage().getTexture();
}


////////////////////////////////////////////////////////////
const float Font::getUnderlinePosition() const
{
    auto face = m_fontHandles ? m_fontHandles->face.get() : nullptr;

    if (face and m_characterSize > 0)
    {
        // Note: Return a fixed position if font is a bitmap font
        if (not m_freeType2Wrapper->isScalable(face))
        {
            return static_cast<float>(m_characterSize) / 10.f;
        }
        return -static_cast<float>(m_freeType2Wrapper->multiplierFix(face->underline_position, face->size->metrics.y_scale)) /
               static_cast<float>(1 << 6);
    }
    else
    {
        return 0.f;
    }
}


////////////////////////////////////////////////////////////
const float Font::getUnderlineThickness() const
{
    auto face = m_fontHandles ? m_fontHandles->face.get() : nullptr;

    if (face and m_characterSize > 0)
    {
        // Note: Return a fixed thickness if font is a bitmap font
        if (not m_freeType2Wrapper->isScalable(face))
        {
            return static_cast<float>(m_characterSize) / 14.f;
        }

        return static_cast<float>(m_freeType2Wrapper->multiplierFix(face->underline_thickness, face->size->metrics.y_scale)) /
               static_cast<float>(1 << 6);
    }
    else
    {
        return 0.f;
    }
}


////////////////////////////////////////////////////////////
const Glyph& Font::getGlyph(const std::uint32_t codePoint, const bool bold, const float outlineThickness) const
{
    Page& page = loadPage();
    const GlyphTable& glyphs = page.getGlyphTable();

    // Note: Build the key by combining the glyph index (based on code point), bold flag, and outline thickness
    auto fontHandles = m_fontHandles ? m_fontHandles->face.get() : nullptr;
    const std::uint64_t key = combine(outlineThickness, bold,  m_freeType2Wrapper->getCharIndex(fontHandles, codePoint));

    if (auto it = glyphs.find(key); it != glyphs.end())
    {
        return it->second;
    }
    else
    {
        Glyph glyph = loadGlyph(codePoint, bold, outlineThickness);
        page.addGlyph(key, glyph);
        return page.getGlyphTable().at(key);
    }
}


////////////////////////////////////////////////////////////
float Font::getLineSpacing() const
{
    auto face = m_fontHandles ? m_fontHandles->face.get() : nullptr;

    if (face and m_characterSize > 0)
    {
        return static_cast<float>(face->size->metrics.height) / static_cast<float>(1 << 6);
    }
    else
    {
        return 0.f;
    }
}


////////////////////////////////////////////////////////////
float Font::getKerning(std::uint32_t first, std::uint32_t second, bool bold) const
{
    auto areCodePointsNonZero = (first != 0 and second != 0);
    auto face = m_fontHandles ? m_fontHandles->face.get() : nullptr;

    if (face and m_characterSize > 0 and areCodePointsNonZero)
    {
        FT_UInt index1 = m_freeType2Wrapper->getCharIndex(face, first);
        FT_UInt index2 = m_freeType2Wrapper->getCharIndex(face, second);

        // Note: Retrieve position compensation deltas generated by FT_LOAD_FORCE_AUTOHINT flag
        auto firstRsbDelta  = static_cast<float>(getGlyph(first, bold).rsbDelta);
        auto secondLsbDelta = static_cast<float>(getGlyph(second, bold).lsbDelta);
        FT_Vector kerning;
        kerning.x = kerning.y = 0;

        if (m_freeType2Wrapper->hasKerning(face))
        {
            m_freeType2Wrapper->getKerning(face, index1, index2, FT_KERNING_UNFITTED, &kerning);
        }

        // Note: X advance is already in pixels for bitmap fonts
        if (not m_freeType2Wrapper->isScalable(face))
        {
            return static_cast<float>(kerning.x);
        }

        // Note: Combine kerning with compensation deltas and return the X advance
        // Note: Flooring is required as we use FT_KERNING_UNFITTED flag which is not quantized in 64 based grid
        return std::floor(
            (secondLsbDelta - firstRsbDelta + static_cast<float>(kerning.x) + 32) / static_cast<float>(1 << 6));
    }
    else
    {
        return 0.f;
    }
}


////////////////////////////////////////////////////////////
const unsigned int Font::getCharacterSize() const
{
    return m_characterSize;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
bool Font::trySetCharacterSize(const unsigned int newCharacterSize)
{
    // Note:
    /*
        // FT_Set_Pixel_Sizes is an expensive function, so we must call it
        // only when necessary to avoid killing performances

        // m_fontHandles and m_fontHandles->face are checked to be non-null before calling this method
    */

    if (m_characterSize != newCharacterSize)
    {
        if (not m_fontHandles)
        {
            LOG_WARNING << "font handles not initialized";
            return false;
        }
    
        auto face = m_fontHandles->face.get();
        int result = m_freeType2Wrapper->setPixelSizes(face, 0, newCharacterSize);

        if (result == FT_Err_Invalid_Pixel_Size)
        {
            // Note: In the case of bitmap fonts, resizing can fail if the requested size is not available
            if (not m_freeType2Wrapper->isScalable(face))
            {
                LOG_WARNING << "Failed to set bitmap";

                std::string aSize{};
                for (std::size_t i = 0; i < face->num_fixed_sizes; ++i)
                {
                    const long size = (face->available_sizes[i].y_ppem + 32) >> 6;
                    aSize.append(std::to_string(size));
                    aSize.append(", ");
                }

                if (not aSize.empty())
                {
                    LOG_DEBUG << "Available sizes are: {" << aSize << "}";
                }
            }
        }

        if (result == FT_Err_Ok)
        {
            m_characterSize = newCharacterSize;
        }

        return result == FT_Err_Ok;
    }
    return true;
}


////////////////////////////////////////////////////////////
Glyph Font::loadGlyph(const std::uint32_t codePoint, const bool bold, const float outlineThickness) const
{
    Glyph glyph;

    if (not m_fontHandles)
    {
        return glyph;
    }

    auto face = m_fontHandles->face.get();

    if (not face and m_characterSize > 0)
    {
        return glyph;
    }

    // Note: Load the glyph corresponding to the code point
    FT_Int32 flags = FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT;

    if (outlineThickness != 0)
    {
        flags |= FT_LOAD_NO_BITMAP;
    }

    auto ftLoadedChar = m_freeType2Wrapper->loadChar(face, codePoint, flags);
    FT_Glyph glyphDesc;
    auto ftGlyphDesc = m_freeType2Wrapper->getGlyph(face->glyph, &glyphDesc);

    if (ftLoadedChar != 0 or ftGlyphDesc != 0)
    {
        return glyph;
    }

    FT_Pos weight  = 1 << 6;
    bool outline = (glyphDesc->format == FT_GLYPH_FORMAT_OUTLINE);
    applyBoldAndOutline(glyphDesc, weight, bold, outline, outlineThickness);
    m_freeType2Wrapper->glyphToBitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, nullptr, 1);
    auto       bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyphDesc);
    FT_Bitmap& bitmap      = bitmapGlyph->bitmap;

    glyphToBitmap(bitmap, weight, bold, outline, outlineThickness);

    // Note: Compute the glyph's advance offset
    glyph.advance = static_cast<float>(bitmapGlyph->root.advance.x >> 16);

    if (bold)
    {
        glyph.advance += static_cast<float>(weight) / static_cast<float>(1 << 6);       
    }

    glyph.lsbDelta = static_cast<int>(face->glyph->lsb_delta);
    glyph.rsbDelta = static_cast<int>(face->glyph->rsb_delta);

    unsigned int width  = bitmap.width;
    unsigned int height = bitmap.rows;

    if ((width > 0) and (height > 0))
    {
        // Leave a small padding around characters, so that filtering doesn't
        // pollute them with pixels from neighbors
        const unsigned int padding = 2;

        width += 2 * padding;
        height += 2 * padding;

        // Get the glyphs page corresponding to the character size
        Page& page = loadPage();

        // Find a good position for the new glyph into the texture
        glyph.textureRect = findGlyphRect(page, {width, height});

        // Make sure the texture data is positioned in the center
        // of the allocated texture rectangle
        glyph.textureRect.p1.x += static_cast<int>(padding);
        glyph.textureRect.p1.y += static_cast<int>(padding);
        glyph.textureRect.p2.x -= static_cast<int>(2 * padding);
        glyph.textureRect.p2.y -= static_cast<int>(2 * padding);

        // Compute the glyph's bounding box
        glyph.bounds.p1.x   = static_cast<float>(bitmapGlyph->left);
        glyph.bounds.p1.y    = static_cast<float>(-bitmapGlyph->top);
        glyph.bounds.p2.x  = static_cast<float>(bitmap.width);
        glyph.bounds.p2.y = static_cast<float>(bitmap.rows);

        // Resize the pixel buffer to the new size and fill it with transparent white pixels
        m_pixelBuffer.resize(static_cast<std::size_t>(width * height * 4));
        std::uint8_t* current = m_pixelBuffer.data();
        std::uint8_t* end = current + width * height * 4;

        auto fontOutlineVisible = 0;
        while (current != end)
        {
            (*current++) = 255;
            (*current++) = 255;
            (*current++) = 255;
            (*current++) = fontOutlineVisible;
        }

        // Extract the glyph's pixels from the bitmap
        const std::uint8_t* pixels = bitmap.buffer;
    
        if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
        {
            // Pixels are 1 bit monochrome values
            for (unsigned int y = padding; y < height - padding; ++y)
            {
                for (unsigned int x = padding; x < width - padding; ++x)
                {
                    // The color channels remain white, just fill the alpha channel
                    std::size_t index            = x + y * width;
                    m_pixelBuffer[index * 4 + 3] = ((pixels[(x - padding) / 8]) & (1 << (7 - ((x - padding) % 8)))) ? 255 : 0;
                }
                pixels += bitmap.pitch;
            }
        }
        else
        {
            // Pixels are 8 bits gray levels
            for (unsigned int y = padding; y < height - padding; ++y)
            {
                for (unsigned int x = padding; x < width - padding; ++x)
                {
                    // The color channels remain white, just fill the alpha channel
                    std::size_t index            = x + y * width;
                    m_pixelBuffer[index * 4 + 3] = pixels[x - padding];
                }
                pixels += bitmap.pitch;
            }
        }

        // Write the pixels to the texture
        unsigned int x = static_cast<unsigned int>(glyph.textureRect.p1.x) - padding;
        unsigned int y = static_cast<unsigned int>(glyph.textureRect.p1.y) - padding;
        unsigned int w = static_cast<unsigned int>(glyph.textureRect.p2.x) + 2 * padding;
        unsigned int h = static_cast<unsigned int>(glyph.textureRect.p2.y) + 2 * padding;
        page.getTexture()->update(m_pixelBuffer.data(), {w, h}, {x, y});
    }

    m_freeType2Wrapper->doneGlyph(glyphDesc);

    return glyph;
}


//////////////////////////////////////////////////////////////////////
bool Font::checkFilePathString(const std::filesystem::path& filepath)
{
    if (not filepath.empty() and filepath.string().c_str()[0] == '\\')
    {
        return true;
    }
    
    LOG_WARNING << "Wrong filepath: " << filepath;
    m_freeType2Wrapper->setErrorCode(1001);
    return false;
}


////////////////////////////////////////////////////////////
geometry::Line<float> Font::findGlyphRect(Page& page, const geometry::Size2Dui& size) const
{
    bool isRowFound{false};
    size_t index = 0;
    const std::vector<Row>& pagesRows = page.getRows();

    for (index ; index < pagesRows.size() ; ++index)
    {
        float ratio = static_cast<float>(size.height) / static_cast<float>(pagesRows[index].height);   
        // Ignore rows that are either too small or too high
        if ((ratio < 0.7f) or (ratio > 1.f))
        {
            continue;
        }

        // Check if there's enough horizontal space left in the row
        if (size.width > page.getTexture()->getSize().width - pagesRows[index].width)
        {
            continue;
        }

        // Make sure that this new row is the best found so far
        if (ratio < 0)
        {
            continue;
        }
        isRowFound = true;
        break;
    }

    if (not isRowFound)
    {
        unsigned int rowHeight = size.height + size.height / 10;
        while ((page.getPositionOfNextRow() + rowHeight >= page.getTexture()->getSize().height) || (size.width >= page.getTexture()->getSize().width))
        {
            // Not enough space: resize the texture if possible
            geometry::Size2D textureSize{page.getTexture()->getSize().width, page.getTexture()->getSize().height};
            auto textureMaxSize = page.getTexture()->getMaxTextureSize();
            if ((textureSize.width * 2u <= textureMaxSize) and (textureSize.height * 2u <= textureMaxSize))
            {
                // Make the texture 2 times bigger
                gfx::render::Texture newTexture;
                if (not newTexture.create(textureSize * static_cast<std::uint16_t>(2)))
                {
                    LOG_ERROR << "Failed to create new page texture";
                    return {gfx::geometry::Point2d<float>(0.f, 0.f), gfx::geometry::Point2d<float>(2.f, 2.f)};
                }

                newTexture.setSmooth(m_isSmooth);
                newTexture.update(*(page.getTexture().get()));
                page.getTexture()->swap(newTexture);
            }
            else
            {
                // Oops, we've reached the maximum texture size...
                LOG_ERROR << "Failed to add a new character to the font: the maximum texture size has been reached";
                return {gfx::geometry::Point2d<float>(0.f, 0.f), gfx::geometry::Point2d<float>(2.f, 2.f)};
            }
        }

        index = pagesRows.size();
        page.addRowHeight(rowHeight);
    }

    geometry::Line<float> rect{{static_cast<float>(pagesRows[index].width), static_cast<float>(pagesRows[index].top)}, {static_cast<float>(size.width), static_cast<float>(size.height)}};

    auto widthToSet = page.getRows()[index].width + size.width;
    page.setWidthForRow(index, widthToSet);

    return rect;
}


////////////////////////////////////////////////////////////
void Font::applyBoldAndOutline(FT_Glyph& glyphDesc, const FT_Pos weight, const bool bold, const bool outline, const float outlineThickness) const
{
    // Note: Apply bold and outline (there is no fallback for outline) if necessary -- first technique using outline (highest quality)
    if (outline)
    {
        if (bold)
        {
            auto outlineGlyph = reinterpret_cast<FT_OutlineGlyph>(glyphDesc);
            m_freeType2Wrapper->outlineEmbolden(&outlineGlyph->outline, weight);
        }

        if (outlineThickness != 0.f)
        {
            auto stroker = m_fontHandles->stroker.get();

            m_freeType2Wrapper->strokerSet(stroker,
                           static_cast<FT_Fixed>(outlineThickness * static_cast<float>(1 << 6)),
                           FT_STROKER_LINECAP_ROUND,
                           FT_STROKER_LINEJOIN_ROUND,
                           0);
            m_freeType2Wrapper->glyphStroke(&glyphDesc, stroker, true);
        }
    }  
}


////////////////////////////////////////////////////////////
void Font::glyphToBitmap(FT_Bitmap& bitmap, const FT_Pos weight, const bool bold, const bool outline, const float outlineThickness) const
{
    // Note:
    /*
        Convert the glyph to a bitmap (i.e. rasterize it)
        Warning! After this line, do not read any data from glyphDesc directly, use
        bitmapGlyph.root to access the FT_Glyph data.
    */

    // Note: Apply bold if necessary -- fallback technique using bitmap (lower quality)
    if (not outline)
    {
        if (bold)
        {
            m_freeType2Wrapper->bitmapEmbolden(m_fontHandles->library.get(), &bitmap, weight, weight);
        }

        if (outlineThickness != 0)
        {
            LOG_WARNING << "Failed to outline glyph (no fallback available)";
        }
    }
}


////////////////////////////////////////////////////////////
he::gfx::text::Page& Font::loadPage() const
{
    if (m_pages.find(m_characterSize) == m_pages.end())
    {
        he::gfx::text::Page page;
        page.createTextureWithSmooth();
        auto it = m_pages.try_emplace(m_characterSize, page);

        if(it.second)
        {
            return it.first->second;
        }
    }

    return m_pages.at(m_characterSize);
}


////////////////////////////////////////////////////////////
bool Font::initializeFreeType(const std::string& path)
{
    cleanup();

    // Note: 
    /*
        Initialize FreeType for every font instance in order to avoid having a single
        global manager that would create a lot of issues regarding creation and destruction order.
    */

    auto fontHandles = std::make_unique<FontHandles>(*m_freeType2Wrapper);

    if (fontHandles->initFreeType() and fontHandles->loadFontFace(path) and fontHandles->loadStoker() and fontHandles->setUnicodeChar())
    {
        m_fontHandles = std::move(fontHandles);
        m_info = m_fontHandles->face->family_name ? m_fontHandles->face->family_name : "";
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
void Font::cleanup()
{
    // Note: Drop ownership of shared FreeType pointers
    m_fontHandles.reset();
    m_pages.clear();
    std::vector<std::uint8_t>().swap(m_pixelBuffer);
}
} // namespace he::gfx::text