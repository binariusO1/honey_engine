#pragma once

#include <filesystem>
#include <memory>
#include <unordered_map>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_STROKER_H
#include "graphics/Texture.hpp"
#include "text/Glyph.hpp"

namespace gfx
{
namespace resources
{
class FontHandles;
struct Page;

using PageTable = std::unordered_map<unsigned int, Page>; //!< Table mapping a character size to its page (texture)

class Font
{
public:
    Font();
    Font(const Font& copy);
    ~Font();

public:
    bool loadFromFile(const std::filesystem::path&);

public:
    const gfx::resources::Texture& getTexture(unsigned int) const;
    float getUnderlinePosition(unsigned int) const;
    float getUnderlineThickness(unsigned int) const;
    const Glyph& getGlyph(std::uint32_t codePoint, unsigned int characterSize, bool bold, float outlineThickness = 0) const;
    float getLineSpacing(unsigned int characterSize) const;
    float getKerning(std::uint32_t first, std::uint32_t second, unsigned int characterSize, bool bold) const;

public:
    bool setCurrentSize(unsigned int) const;

private:
    void cleanup();
    Page& loadPage(unsigned int) const;
    Glyph loadGlyph(std::uint32_t codePoint, unsigned int characterSize, bool bold, float outlineThickness) const;
    geometry::Line<float> findGlyphRect(Page& page, const geometry::Size2Dui& size) const;

public:
    struct Info
    {
        std::string family; //!< The font family
    };

public:
    std::shared_ptr<FontHandles>        m_fontHandles;    //!< Shared information about the internal font instance
    bool                                m_isSmooth{true}; //!< Status of the smooth filter
    Info                                m_info;           //!< Information about the font
    mutable PageTable                   m_pages;          //!< Table containing the glyphs pages by character size
    mutable std::vector<std::uint8_t>   m_pixelBuffer{}; //!< Pixel buffer holding a glyph's pixels before being written to the texture
};

class FontHandles
{
private:
    // Default constructible deleter functor
    struct Deleter
    {
        void operator()(FT_Library theLibrary)
        {
            FT_Done_FreeType(theLibrary);
        }
        void operator()(FT_Face theFace)
        {
            FT_Done_Face(theFace);
        }
        void operator()(FT_Stroker theStroker)
        {
            FT_Stroker_Done(theStroker);
        }
    };

public:
    std::unique_ptr<std::remove_pointer_t<FT_Library>, Deleter> library;   //< Pointer to the internal library interface
    std::unique_ptr<FT_StreamRec>                               streamRec; //< Pointer to the stream rec instance
    std::unique_ptr<std::remove_pointer_t<FT_Face>, Deleter>    face;      //< Pointer to the internal font face
    std::unique_ptr<std::remove_pointer_t<FT_Stroker>, Deleter> stroker;   //< Pointer to the stroker
};

} // namespace resources
} // namespace gfx