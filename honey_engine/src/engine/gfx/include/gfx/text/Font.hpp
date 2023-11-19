#pragma once

#include "gfx/geometry/Size2d.hpp"
#include "gfx/text/IFont.hpp"
#include "gfx/text/IFreeType2Wrapper.hpp"

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

class Font : public IFont
{
public:
    Font();
    Font(const Font& copy);
    ~Font() override = default;

public:
    bool loadFromFile(const std::filesystem::path& filepath) override;
    bool setCharacterSize(const unsigned int characterSize) override;
    const std::shared_ptr<he::gfx::render::Texture> getTexture() const override;
    const float getUnderlinePosition() const override;
    const float getUnderlineThickness() const override;
    const Glyph& getGlyph(const std::uint32_t codePoint, const bool bold, const float outlineThickness = 0) const override;
    float getLineSpacing() const override;
    float getKerning(std::uint32_t first, std::uint32_t second, bool bold) const override;
    const unsigned int getCharacterSize() const override;

private:
    bool trySetCharacterSize(const unsigned int newCharacterSize);
    void cleanup();
    he::gfx::text::Page& loadPage() const override;
    bool initializeFreeType(const std::string& path);
    Glyph loadGlyph(const std::uint32_t codePoint, const bool bold, const float outlineThickness) const;
    void applyBoldAndOutline(FT_Glyph&, const FT_Pos weight, const bool bold, const bool outline, const float outlineThickness) const;
    void glyphToBitmap(FT_Bitmap&, const FT_Pos weight, const bool bold, const bool outline, const float outlineThickness) const;
    geometry::Line<float> findGlyphRect(Page& page, const geometry::Size2Dui& size) const;
    bool checkFilePathString(const std::filesystem::path& filepath);

private:
    bool m_isSmooth{true};
    std::shared_ptr<FontHandles> m_fontHandles{nullptr};        //!< Shared information about the internal font instance
    std::string m_info;                                         //!< Information about the font
    mutable PageTable m_pages{};                           //!< Table containing the glyphs pages by character size
    mutable std::vector<std::uint8_t> m_pixelBuffer{};      //!< Pixel buffer holding a glyph's pixels before being written to the texture
    unsigned int m_characterSize{0};

protected:
    std::shared_ptr<he::gfx::text::IFreeType2Wrapper> m_freeType2Wrapper;
};
} // namespace text
} // namespace gfx
} // namespace he