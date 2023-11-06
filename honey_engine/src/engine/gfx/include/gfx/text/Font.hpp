#pragma once

#include <filesystem>
#include <memory>
#include <unordered_map>
#include <ft2build.h>
#include <vector>
#include "gfx/text/Page.hpp"
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_STROKER_H

namespace he
{
namespace gfx
{
namespace render
{
class Texture;
} // namespace render
namespace text
{
class FontHandles;
struct Glyph;

// Note:  Table mapping a character size to its page (texture)
using PageTable = std::unordered_map<unsigned int, Page>;

class Font
{
public:
    Font();
    Font(const std::string& filepath);
    Font(const Font& copy) = default;
    ~Font() = default;

public:
    bool setCurrentSize(const unsigned int) const;
    bool loadFromFile(const std::filesystem::path& filepath);
    const std::shared_ptr<he::gfx::render::Texture> getTexture(const unsigned int characterSize) const;
    const float getUnderlinePosition(const unsigned int characterSize) const;
    const float getUnderlineThickness(const unsigned int) const;
    const Glyph& getGlyph(const std::uint32_t codePoint, const unsigned int characterSize, const bool bold, const float outlineThickness = 0) const;
    float getLineSpacing(unsigned int characterSize) const;
    float getKerning(std::uint32_t first, std::uint32_t second, unsigned int characterSize, bool bold) const;

private:
    void cleanup();
    he::gfx::text::Page& loadPage(const unsigned int) const;
    bool initializeFreeType(const std::string& path);
    Glyph loadGlyph(const std::uint32_t codePoint, const unsigned int characterSize, const bool bold, const float outlineThickness) const;
    void applyBoldAndOutline(FT_Glyph&, const FT_Pos weight, const bool bold, const bool outline, const float outlineThickness) const;
    void glyphToBitmap(FT_Bitmap&, const FT_Pos weight, const bool bold, const bool outline, const float outlineThickness) const;
    geometry::Line<float> findGlyphRect(Page& page, const geometry::Size2Dui& size) const;

private:
    class FontHandles
    {
    private:
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
        bool initFreeType();
        bool loadFontFace(const std::string& path);
        bool loadStoker();
        bool setUnicodeChar();

    private:
        friend class he::gfx::text::Font;
        std::unique_ptr<std::remove_pointer_t<FT_Library>, Deleter> library{nullptr};   //< Pointer to the internal library interface
        std::unique_ptr<FT_StreamRec>                               streamRec{nullptr}; //< Pointer to the stream rec instance
        std::unique_ptr<std::remove_pointer_t<FT_Face>, Deleter>    face{nullptr};      //< Pointer to the internal font face
        std::unique_ptr<std::remove_pointer_t<FT_Stroker>, Deleter> stroker{nullptr};   //< Pointer to the stroker
    };

private:
    bool m_isSmooth{true};
    std::shared_ptr<FontHandles> m_fontHandles{nullptr};        //!< Shared information about the internal font instance
    std::string m_info;                                         //!< Information about the font
    mutable PageTable m_pages{};                           //!< Table containing the glyphs pages by character size
    mutable std::vector<std::uint8_t> m_pixelBuffer{};      //!< Pixel buffer holding a glyph's pixels before being written to the texture
};
} // namespace text
} // namespace gfx
} // namespace he