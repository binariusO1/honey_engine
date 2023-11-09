#include "gfx/text/Font.hpp"

#include <cmath>
#include <optional>
#include "gfx/geometry/Line.hpp"
#include "gfx/geometry/Size2d.hpp"
#include "gfx/text/Glyph.hpp"
#include "exception/invalid_initialization.hpp"
#include "logger/Logger.hpp"

const char* projectPath = "D:\\Projects\\CPP\\games\\honey_engine\\";
const char* loadFontErr = "Unable to load font!";

namespace
{
//////////////////////////////////////////////////////////////////////
std::string getAbsolutePath(const std::filesystem::path& filepath)
{
    std::string path = filepath.string().c_str();
    return projectPath + path;
}


//////////////////////////////////////////////////////////////////////
bool checkFilePath(const std::filesystem::path& filepath)
{
    if (not filepath.empty() and filepath.string().c_str()[0] == '\\')
    {
        return true;
    }
    LOG_WARNING << "Wrong filepath: " << filepath;
    return false;
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
class FontHandles;


//////////////////////////////////////////////////////////////////////
Font::Font()
{
    const char * defaultFontPath = "\\data\\gfx\\fonts\\calibri.ttf";
    if (not loadFromFile(defaultFontPath))
    {
        throw he::common::invalid_initialization(loadFontErr);
    }
}


//////////////////////////////////////////////////////////////////////
Font::Font(const std::string& filepath)
{
    if (not loadFromFile(filepath))
    {
        throw he::common::invalid_initialization(loadFontErr);
    }
}


////////////////////////////////////////////////////////////
bool Font::loadFromFile(const std::filesystem::path& filepath)
{
    if (checkFilePath(filepath))
    {
        const std::string& absolutePath = getAbsolutePath(filepath);
        if (initializeFreeType(absolutePath))
        {
            return true;
        }
    }
    LOG_ERROR << loadFontErr;
    return false;
}


////////////////////////////////////////////////////////////
const std::shared_ptr<he::gfx::render::Texture> Font::getTexture(const unsigned int characterSize) const
{
    return loadPage(characterSize).getTexture();
}


////////////////////////////////////////////////////////////
const float Font::getUnderlinePosition(const unsigned int characterSize) const
{
    auto face = m_fontHandles ? m_fontHandles->face.get() : nullptr;

    if (face and setCurrentSize(characterSize))
    {
        // Note: Return a fixed position if font is a bitmap font
        if ( not FT_IS_SCALABLE(face))
        {
            return static_cast<float>(characterSize) / 10.f;
        }

        return -static_cast<float>(FT_MulFix(face->underline_position, face->size->metrics.y_scale)) /
               static_cast<float>(1 << 6);
    }
    else
    {
        return 0.f;
    }
}


////////////////////////////////////////////////////////////
bool Font::setCurrentSize(const unsigned int characterSize) const
{
    // Note:
    /*
        // FT_Set_Pixel_Sizes is an expensive function, so we must call it
        // only when necessary to avoid killing performances

        // m_fontHandles and m_fontHandles->face are checked to be non-null before calling this method
    */
    auto face = m_fontHandles->face.get();
    FT_UShort currentSize = face->size->metrics.x_ppem;

    if (currentSize != characterSize)
    {
        FT_Error result = FT_Set_Pixel_Sizes(face, 0, characterSize);

        if (result == FT_Err_Invalid_Pixel_Size)
        {
            // Note: In the case of bitmap fonts, resizing can fail if the requested size is not available
            if (not FT_IS_SCALABLE(face))
            {
                LOG_WARNING << "Failed to set bitmap font size to: " << characterSize;
                LOG_DEBUG << "Available sizes are: ";
                std::string aSize{};
                for (std::size_t i = 0; i < face->num_fixed_sizes; ++i)
                {
                    const long size = (face->available_sizes[i].y_ppem + 32) >> 6;
                    aSize += size;
                }
                LOG_ERROR << aSize;
            }
            else
            {
                LOG_WARNING << "Failed to set font size to: " << characterSize;
            }
        }
        return result == FT_Err_Ok;
    }

    return true;
}


////////////////////////////////////////////////////////////
const float Font::getUnderlineThickness(const unsigned int characterSize) const
{
    auto face = m_fontHandles ? m_fontHandles->face.get() : nullptr;

    if (face and setCurrentSize(characterSize))
    {
        // Note: Return a fixed thickness if font is a bitmap font
        if (not FT_IS_SCALABLE(face))
        {
            return static_cast<float>(characterSize) / 14.f;
        }

        return static_cast<float>(FT_MulFix(face->underline_thickness, face->size->metrics.y_scale)) /
               static_cast<float>(1 << 6);
    }
    else
    {
        return 0.f;
    }
}


////////////////////////////////////////////////////////////
const Glyph& Font::getGlyph(const std::uint32_t codePoint, const unsigned int characterSize, const bool bold, const float outlineThickness) const
{
    Page& page = loadPage(characterSize);
    const GlyphTable& glyphs = page.getGlyphTable();

    // Note: Build the key by combining the glyph index (based on code point), bold flag, and outline thickness
    const std::uint64_t key = combine(outlineThickness, bold, FT_Get_Char_Index(m_fontHandles ? m_fontHandles->face.get() : nullptr, codePoint));

    if (auto it = glyphs.find(key); it != glyphs.end())
    {
        return it->second;
    }
    else
    {
        Glyph glyph = loadGlyph(codePoint, characterSize, bold, outlineThickness);
        page.addGlyph(key, glyph);
        return page.getGlyphTable().at(key);
    }
}


////////////////////////////////////////////////////////////
float Font::getLineSpacing(unsigned int characterSize) const
{
    auto face = m_fontHandles ? m_fontHandles->face.get() : nullptr;

    if (face and setCurrentSize(characterSize))
    {
        return static_cast<float>(face->size->metrics.height) / static_cast<float>(1 << 6);
    }
    else
    {
        return 0.f;
    }
}


////////////////////////////////////////////////////////////
float Font::getKerning(std::uint32_t first, std::uint32_t second, unsigned int characterSize, bool bold) const
{
    // Special case where first or second is 0 (null character)
    if (first == 0 || second == 0)
        return 0.f;

    auto face = m_fontHandles ? m_fontHandles->face.get() : nullptr;

    if (face && setCurrentSize(characterSize))
    {
        // Convert the characters to indices
        FT_UInt index1 = FT_Get_Char_Index(face, first);
        FT_UInt index2 = FT_Get_Char_Index(face, second);

        // Retrieve position compensation deltas generated by FT_LOAD_FORCE_AUTOHINT flag
        auto firstRsbDelta  = static_cast<float>(getGlyph(first, characterSize, bold).rsbDelta);
        auto secondLsbDelta = static_cast<float>(getGlyph(second, characterSize, bold).lsbDelta);

        // Get the kerning vector if present
        FT_Vector kerning;
        kerning.x = kerning.y = 0;
        if (FT_HAS_KERNING(face))
        {
            FT_Get_Kerning(face, index1, index2, FT_KERNING_UNFITTED, &kerning);
        }

        // X advance is already in pixels for bitmap fonts
        if (!FT_IS_SCALABLE(face))
            return static_cast<float>(kerning.x);

        // Combine kerning with compensation deltas and return the X advance
        // Flooring is required as we use FT_KERNING_UNFITTED flag which is not quantized in 64 based grid
        return std::floor(
            (secondLsbDelta - firstRsbDelta + static_cast<float>(kerning.x) + 32) / static_cast<float>(1 << 6));
    }
    else
    {
        LOG_WARNING << "Invalid font!";
        return 0.f;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
Glyph Font::loadGlyph(const std::uint32_t codePoint, const unsigned int characterSize, const bool bold, const float outlineThickness) const
{
    Glyph glyph;

    if (not m_fontHandles)
    {
        return glyph;
    }
    auto face = m_fontHandles->face.get();
    auto currentSize = not setCurrentSize(characterSize);

    if (not face and not currentSize)
    {
        return glyph;
    }

    // Note: Load the glyph corresponding to the code point
    FT_Int32 flags = FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT;
    if (outlineThickness != 0)
    {
        flags |= FT_LOAD_NO_BITMAP;
    }

    auto ftLoadedChar = FT_Load_Char(face, codePoint, flags);
    FT_Glyph glyphDesc;
    auto ftGlyphDesc = FT_Get_Glyph(face->glyph, &glyphDesc);

    if (ftLoadedChar != 0 or ftGlyphDesc != 0)
    {
        return glyph;
    }

    FT_Pos weight  = 1 << 6;
    bool outline = (glyphDesc->format == FT_GLYPH_FORMAT_OUTLINE);
    applyBoldAndOutline(glyphDesc, weight, bold, outline, outlineThickness);

    FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, nullptr, 1);
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
        Page& page = loadPage(characterSize);

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

    FT_Done_Glyph(glyphDesc);

    return glyph;
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
            FT_Outline_Embolden(&outlineGlyph->outline, weight);
        }

        if (outlineThickness != 0.f)
        {
            auto stroker = m_fontHandles->stroker.get();

            FT_Stroker_Set(stroker,
                           static_cast<FT_Fixed>(outlineThickness * static_cast<float>(1 << 6)),
                           FT_STROKER_LINECAP_ROUND,
                           FT_STROKER_LINEJOIN_ROUND,
                           0);
            FT_Glyph_Stroke(&glyphDesc, stroker, true);
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
            FT_Bitmap_Embolden(m_fontHandles->library.get(), &bitmap, weight, weight);
        }

        if (outlineThickness != 0)
        {
            LOG_WARNING << "Failed to outline glyph (no fallback available)";
        }
    }
}


////////////////////////////////////////////////////////////
he::gfx::text::Page& Font::loadPage(const unsigned int characterSize) const
{
    if (m_pages.find(characterSize) == m_pages.end())
    {
        he::gfx::text::Page page;
        page.createTextureWithSmooth();
        auto it = m_pages.try_emplace(characterSize, page);

        if(it.second)
        {
            return it.first->second;
        }
    }

    return m_pages.at(characterSize);
}


////////////////////////////////////////////////////////////
bool Font::initializeFreeType(const std::string& path)
{
    // Note: Cleanup the previous resources
    cleanup();

    // Note: 
    /*
        Initialize FreeType for every font instance in order to avoid having a single
        global manager that would create a lot of issues regarding creation and destruction order.
    */

    auto fontHandles = std::make_unique<FontHandles>();

    if (fontHandles->initFreeType() and fontHandles->loadFontFace(path) and fontHandles->loadStoker() and fontHandles->setUnicodeChar())
    {
        m_fontHandles = std::move(fontHandles);
        m_info = m_fontHandles->face->family_name ? m_fontHandles->face->family_name : std::string();
        return true;
    }
    LOG_WARNING << "Freetype not initialized!";
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


////////////////////////////////////////////////////////////
bool Font::FontHandles::initFreeType()
{
    FT_Library ftLibrary;
    if (FT_Init_FreeType(&ftLibrary) != 0)
    {
        LOG_WARNING << "Failed to initialize FreeType";
        return false;
    }
    library.reset(ftLibrary);
    return true;
}


////////////////////////////////////////////////////////////
bool Font::FontHandles::loadFontFace(const std::string& path)
{
    // Note: Load the new font face from the specified file
    FT_Face ftFace;
    if (FT_New_Face(library.get(), path.c_str(), 0, &ftFace) != 0)
    {
        LOG_WARNING << "Failed to create the font face";
        return false;
    }
    face.reset(ftFace);
    return true;    
}


////////////////////////////////////////////////////////////
bool Font::FontHandles::loadStoker()
{
    // Note: Load the stroker that will be used to outline the font
    FT_Stroker ftStroker;
    if (FT_Stroker_New(library.get(), &ftStroker) != 0)
    {
        LOG_WARNING << "Failed to create the stroker";
        return false;
    }
    stroker.reset(ftStroker);
    return true;    
}


////////////////////////////////////////////////////////////
bool Font::FontHandles::setUnicodeChar()
{
    // Note: Select the unicode character map
    if (FT_Select_Charmap(face.get(), FT_ENCODING_UNICODE) != 0)
    {
        LOG_WARNING << "Failed to set the Unicode character set";
        return false;
    }
    return true;
}
} // namespace he::gfx::text