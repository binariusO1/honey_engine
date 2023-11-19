#pragma once

#include <filesystem>
#include "gfx/text/Page.hpp"

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
class IFont
{
public:
    virtual ~IFont() = default;

public:
    virtual const std::shared_ptr<he::gfx::render::Texture> getTexture() const = 0;
    virtual bool setCharacterSize(const unsigned int characterSize) = 0;
    virtual bool loadFromFile(const std::filesystem::path& filepath) = 0;
    virtual const float getUnderlinePosition() const = 0;
    virtual const float getUnderlineThickness() const = 0;
    virtual const Glyph& getGlyph(const std::uint32_t codePoint, const bool bold, const float outlineThickness = 0) const = 0;
    virtual float getLineSpacing() const = 0;
    virtual float getKerning(std::uint32_t first, std::uint32_t second, bool bold) const = 0;
    virtual const unsigned int getCharacterSize() const = 0;

protected:
    virtual he::gfx::text::Page& loadPage() const = 0;
};
} // namespace text
} // namespace gfx
} // namespace he