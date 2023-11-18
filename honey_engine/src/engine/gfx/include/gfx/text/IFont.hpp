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
    virtual const std::shared_ptr<he::gfx::render::Texture> getTexture(const unsigned int characterSize) const = 0;
    virtual bool loadFromFile(const std::filesystem::path& filepath) = 0;
    virtual const float getUnderlinePosition(const unsigned int characterSize) const = 0;
    virtual const float getUnderlineThickness(const unsigned int characterSize) const = 0;
    virtual const Glyph& getGlyph(const std::uint32_t codePoint, const unsigned int characterSize, const bool bold, const float outlineThickness = 0) const = 0;
    virtual float getLineSpacing(unsigned int characterSize) const = 0;
    virtual float getKerning(std::uint32_t first, std::uint32_t second, unsigned int characterSize, bool bold) const = 0;

protected:
    virtual he::gfx::text::Page& loadPage(const unsigned int) const = 0;
};
} // namespace text
} // namespace gfx
} // namespace he