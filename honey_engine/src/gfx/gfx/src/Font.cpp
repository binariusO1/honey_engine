#include "text/Font.hpp"

#include <cmath>
// #include "sys/config/Config.hpp"
#include "logger/Logger.hpp"
#include "text/Page.hpp"

#define FONT_DEBUG_MODE 0
#define FONT_OUTLINE_VISIBLE 0

namespace
{
std::string getAbsolutePath(const std::string& path) //TODO połaczyć z tym co jest w ImageLoader.cpp
{
   std::string prefix{sys::Config::Get().dev.TEMP_DATA_PATH_PREFIX()};
   return prefix + path;
}

// Helper to intepret memory as a specific type
template <typename T, typename U>
inline T reinterpret(const U& input)
{
    T output;
    memcpy(&output, &input, sizeof(U));
    return output;
}

inline std::string formatDebugPathInfo(const std::filesystem::path& path)
{
    std::ostringstream stream;
    stream << "    Provided path: " << path << '\n';
    stream << "    Absolute path: " << std::filesystem::absolute(path);
    return stream.str();
}
// Combine outline thickness, boldness and font glyph index into a single 64-bit key
std::uint64_t combine(float outlineThickness, bool bold, std::uint32_t index)
{
    return (static_cast<std::uint64_t>(reinterpret<std::uint32_t>(outlineThickness)) << 32) |
           (static_cast<std::uint64_t>(bold) << 31) | index;
}
} // namespace
namespace gfx
{
namespace resources
{
//////////////////////////////////////////////////////////////////////
Font::Font()
{
    LOG_MEMORY << LOG_CONSTRUCTOR;
    loadFromFile("\\data\\fonts\\arial.ttf");
}


////////////////////////////////////////////////////////////
Font::Font(const Font& copy) 
    : m_fontHandles(copy.m_fontHandles)
    , m_isSmooth(copy.m_isSmooth)
    , m_info(copy.m_info)
    , m_pages(copy.m_pages)
    , m_pixelBuffer(copy.m_pixelBuffer)
{
    LOG_MEMORY << LOG_COPY_CONSTRUCTOR;
}


//////////////////////////////////////////////////////////////////////
Font::~Font()
{
    LOG_MEMORY << LOG_DESTRUCTOR;
}


////////////////////////////////////////////////////////////
void Font::cleanup()
{
    // Drop ownership of shared FreeType pointers
    m_fontHandles.reset();

    // Reset members
    m_pages.clear();
    std::vector<std::uint8_t>().swap(m_pixelBuffer);
}


////////////////////////////////////////////////////////////
gfx::resources::Page& Font::loadPage(unsigned int characterSize) const
{
    return m_pages.try_emplace(characterSize, m_isSmooth).first->second;
}


////////////////////////////////////////////////////////////
bool Font::loadFromFile(const std::filesystem::path& path)
{
    std::string absolutePath{};

    if (not path.empty() and path.string().c_str()[0] == '\\') //TODO połaczyć z tym co jest w ImageLoader.cpp
    {
        absolutePath = getAbsolutePath(path.string().c_str());
    }

    // Cleanup the previous resources
    cleanup();

    auto fontHandles = std::make_unique<FontHandles>();

    // Initialize FreeType
    // Note: we initialize FreeType for every font instance in order to avoid having a single
    // global manager that would create a lot of issues regarding creation and destruction order.
    FT_Library library;
    if (FT_Init_FreeType(&library) != 0)
    {
        LOG_ERROR << "Failed to load font (failed to initialize FreeType)\n" << formatDebugPathInfo(absolutePath);
        return false;
    }
    fontHandles->library.reset(library);

    // Load the new font face from the specified file
    FT_Face face;
    if (FT_New_Face(library, absolutePath.c_str(), 0, &face) != 0)
    {
        LOG_ERROR << "Failed to load font (failed to create the font face)\n" << formatDebugPathInfo(absolutePath);
        return false;
    }
    fontHandles->face.reset(face);

    // Load the stroker that will be used to outline the font
    FT_Stroker stroker;
    if (FT_Stroker_New(library, &stroker) != 0)
    {
        LOG_ERROR << "Failed to load font (failed to create the stroker)\n" << formatDebugPathInfo(absolutePath);
        return false;
    }
    fontHandles->stroker.reset(stroker);

    // Select the unicode character map
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0)
    {
        LOG_ERROR << "Failed to load font (failed to set the Unicode character set)\n" << formatDebugPathInfo(absolutePath);
        return false;
    }

    // Store the loaded font handles
    m_fontHandles = std::move(fontHandles);

    // Store the font information
    m_info.family = face->family_name ? face->family_name : std::string();

    return true;
}


////////////////////////////////////////////////////////////
float Font::getUnderlinePosition(unsigned int characterSize) const
{
    auto face = m_fontHandles ? m_fontHandles->face.get() : nullptr;

    if (face and setCurrentSize(characterSize))
    {
        // Return a fixed position if font is a bitmap font
        if (!FT_IS_SCALABLE(face))
            return static_cast<float>(characterSize) / 10.f;

        return -static_cast<float>(FT_MulFix(face->underline_position, face->size->metrics.y_scale)) /
               static_cast<float>(1 << 6);
    }
    else
    {
        return 0.f;
    }
}


////////////////////////////////////////////////////////////
float Font::getUnderlineThickness(unsigned int characterSize) const
{
    auto face = m_fontHandles ? m_fontHandles->face.get() : nullptr;

    if (face && setCurrentSize(characterSize))
    {
        // Return a fixed thickness if font is a bitmap font
        if (!FT_IS_SCALABLE(face))
            return static_cast<float>(characterSize) / 14.f;

        return static_cast<float>(FT_MulFix(face->underline_thickness, face->size->metrics.y_scale)) /
               static_cast<float>(1 << 6);
    }
    else
    {
        return 0.f;
    }
}


////////////////////////////////////////////////////////////
const gfx::resources::Texture& Font::getTexture(unsigned int characterSize) const
{
    return loadPage(characterSize).texture;
}

////////////////////////////////////////////////////////////
const Glyph& Font::getGlyph(std::uint32_t codePoint, unsigned int characterSize, bool bold, float outlineThickness) const
{
#if FONT_DEBUG_MODE
    LOG_DEBUG << "Glyph& Font::getGlyph()" << ", codePoint: " << codePoint << ", characterSize: " << characterSize;
#endif

    // Get the page corresponding to the character size
    GlyphTable& glyphs = loadPage(characterSize).glyphs;

    // Build the key by combining the glyph index (based on code point), bold flag, and outline thickness
    std::uint64_t key = combine(outlineThickness,
                                bold,
                                FT_Get_Char_Index(m_fontHandles ? m_fontHandles->face.get() : nullptr, codePoint));

    // Search the glyph into the cache
    if (auto it = glyphs.find(key); it != glyphs.end())
    {
        // Found: just return it
        return it->second;
    }
    else
    {
        // Not found: we have to load it
        Glyph glyph = loadGlyph(codePoint, characterSize, bold, outlineThickness);
        return glyphs.emplace(key, glyph).first->second;
    }
}


////////////////////////////////////////////////////////////
bool Font::setCurrentSize(unsigned int characterSize) const
{
    // FT_Set_Pixel_Sizes is an expensive function, so we must call it
    // only when necessary to avoid killing performances

    // m_fontHandles and m_fontHandles->face are checked to be non-null before calling this method
    auto      face        = m_fontHandles->face.get();
    FT_UShort currentSize = face->size->metrics.x_ppem;

    if (currentSize != characterSize)
    {
        FT_Error result = FT_Set_Pixel_Sizes(face, 0, characterSize);

        if (result == FT_Err_Invalid_Pixel_Size)
        {
            // In the case of bitmap fonts, resizing can
            // fail if the requested size is not available
            if (not FT_IS_SCALABLE(face))
            {
                LOG_ERROR << "Failed to set bitmap font size to " << characterSize;
                LOG_ERROR<< "Available sizes are: ";
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
                LOG_ERROR << "Failed to set font size to " << characterSize;
            }
        }

        return result == FT_Err_Ok;
    }

    return true;
}


////////////////////////////////////////////////////////////
geometry::Line<float> Font::findGlyphRect(Page& page, const geometry::Size2Dui& size) const
{
#if FONT_DEBUG_MODE
    LOG_DEBUG << "Font::findGlyphRect()";
#endif

    // Find the line that fits well the glyph
    Row*  row       = nullptr;
    float bestRatio = 0;
    for (auto it = page.rows.begin(); it != page.rows.end() && !row; ++it)
    {
        float ratio = static_cast<float>(size.height) / static_cast<float>(it->height);
        // Ignore rows that are either too small or too high
        if ((ratio < 0.7f) or (ratio > 1.f))
        {
            continue;
        }


        // Check if there's enough horizontal space left in the row
        if (size.width > page.texture.getSize().width - it->width)
        {
            continue;
        }


        // Make sure that this new row is the best found so far
        if (ratio < bestRatio)
        {
            continue;
        }

        // The current row passed all the tests: we can select it
        row       = &*it;
        bestRatio = ratio;
    }

    // If we didn't find a matching row, create a new one (10% taller than the glyph)
    if (not row)
    {
        unsigned int rowHeight = size.height + size.height / 10;
        while ((page.nextRow + rowHeight >= page.texture.getSize().height) || (size.width >= page.texture.getSize().width))
        {
            // Not enough space: resize the texture if possible
            geometry::Size2D textureSize{page.texture.getSize().width, page.texture.getSize().height};
            if ((textureSize.width * 2u <= gfx::resources::Texture::getMaximumSize()) and (textureSize.height * 2u <= gfx::resources::Texture::getMaximumSize()))
            {
                // Make the texture 2 times bigger
                gfx::resources::Texture newTexture;
                if (not newTexture.create(textureSize * static_cast<std::uint16_t>(2)))
                {
                    LOG_ERROR << "Failed to create new page texture";
                    return geometry::Line<float>({0, 0}, {2, 2});
                }

                newTexture.setSmooth(m_isSmooth);
                newTexture.update(page.texture);
                page.texture.swap(newTexture);
            }
            else
            {
                // Oops, we've reached the maximum texture size...
                LOG_ERROR << "Failed to add a new character to the font: the maximum texture size has been reached";
                return geometry::Line<float>({0, 0}, {2, 2});
            }
        }

        // We can now create the new row
        page.rows.emplace_back(page.nextRow, rowHeight);
        page.nextRow += rowHeight;
        row = &page.rows.back();
    }

    // Find the glyph's rectangle on the selected row
    geometry::Line<float> rect({static_cast<float>(row->width), static_cast<float>(row->top)}, {static_cast<float>(size.width), static_cast<float>(size.height)});

    // Update the row informations
    row->width += size.width;

    return rect;
}


////////////////////////////////////////////////////////////
Glyph Font::loadGlyph(std::uint32_t codePoint, unsigned int characterSize, bool bold, float outlineThickness) const
{
#if FONT_DEBUG_MODE
    LOG_DEBUG << "Font::loadGlyph()";
#endif
    // The glyph to return
    Glyph glyph;

    // Stop if no font is loaded
    if (not m_fontHandles)
    {
        return glyph;
    }

    // Get our FT_Face
    auto face = m_fontHandles->face.get();
    if (not face)
    {
        return glyph;
    }

    // Set the character size
    if (not setCurrentSize(characterSize))
    {
        return glyph;
    }

    // Load the glyph corresponding to the code point
    FT_Int32 flags = FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT;
    if (outlineThickness != 0)
    {
        flags |= FT_LOAD_NO_BITMAP;
    }

    if (FT_Load_Char(face, codePoint, flags) != 0)
    {
        return glyph;
    }

    // Retrieve the glyph
    FT_Glyph glyphDesc;
    if (FT_Get_Glyph(face->glyph, &glyphDesc) != 0)
    {
        return glyph;      
    }

    // Apply bold and outline (there is no fallback for outline) if necessary -- first technique using outline (highest quality)
    FT_Pos weight  = 1 << 6;
    bool   outline = (glyphDesc->format == FT_GLYPH_FORMAT_OUTLINE);
    if (outline)
    {
        if (bold)
        {
            auto outlineGlyph = reinterpret_cast<FT_OutlineGlyph>(glyphDesc);
            FT_Outline_Embolden(&outlineGlyph->outline, weight);
        }

        if (outlineThickness != 0)
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

    // Convert the glyph to a bitmap (i.e. rasterize it)
    // Warning! After this line, do not read any data from glyphDesc directly, use
    // bitmapGlyph.root to access the FT_Glyph data.
    FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, nullptr, 1);
    auto       bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyphDesc);
    FT_Bitmap& bitmap      = bitmapGlyph->bitmap;

    // Apply bold if necessary -- fallback technique using bitmap (lower quality)
    if (not outline)
    {
        if (bold)
        {
            FT_Bitmap_Embolden(m_fontHandles->library.get(), &bitmap, weight, weight);
        }

        if (outlineThickness != 0)
        {
            LOG_ERROR << "Failed to outline glyph (no fallback available)";
        }
    }

    // Compute the glyph's advance offset
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
        m_pixelBuffer.resize(static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4);

#if FONT_DEBUG_MODE 
        LOG_DEBUG << "&page: " << LOG_GREEN << &page << LOG_WHITE;
        LOG_DEBUG << "glyph.textureRect: " << glyph.textureRect.p1 << " " << glyph.textureRect.p2;
        LOG_DEBUG  << "glyph.textureRect.p1.x: " << glyph.textureRect.p1.x
        << ", glyph.textureRect.p1.y: " << glyph.textureRect.p1.y
        << ", glyph.textureRect.p2.x: " << glyph.textureRect.p2.x
        << ", glyph.textureRect.p2.y: " << glyph.textureRect.p2.y;
        LOG_DEBUG << "glyph.bounds.p1.x: " << glyph.bounds.p1.x
        << ", glyph.bounds.p1.y: " << glyph.bounds.p1.y
        << ", glyph.bounds.p2.x: " << glyph.bounds.p2.x
        << ", glyph.bounds.p2.y: " << glyph.bounds.p2.y << LOG_WHITE;
        LOG_DEBUG << "m_pixelBuffer.size() : " << m_pixelBuffer.size() << ", width: " << width << ", height: " << height << " [pxl]"; 
#endif

        std::uint8_t* current = m_pixelBuffer.data();
        std::uint8_t* end     = current + width * height * 4;

        while (current != end)
        {
            (*current++) = 255;
            (*current++) = 255;
            (*current++) = 255;
#if FONT_OUTLINE_VISIBLE
            (*current++) = 255;
#else
            (*current++) = 0;
#endif
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
        page.texture.update(m_pixelBuffer.data(), {w, h}, {x, y});

#if FONT_DEBUG_MODE
        LOG_DEBUG << "w: " << w << ", h: " << h << ", x: " << x << ", y: " << y;
        LOG_DEBUG << "page.texture.getSize(): " << page.texture.getSize();
        LOG_DEBUG << "glyph.textureRect: " << glyph.textureRect.p1 << ", " << glyph.textureRect.p2;
#endif
    }

    // Delete the FT glyph
    FT_Done_Glyph(glyphDesc);

    // Done :)
    return glyph;
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
            FT_Get_Kerning(face, index1, index2, FT_KERNING_UNFITTED, &kerning);

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
        // Invalid font
        return 0.f;
    }
}


} // namespace resources
} // namespace gfx