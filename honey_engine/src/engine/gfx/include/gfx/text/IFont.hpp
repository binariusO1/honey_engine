#pragma once

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

protected:
    virtual he::gfx::text::Page& loadPage(const unsigned int) const = 0;
};
} // namespace text
} // namespace gfx
} // namespace he