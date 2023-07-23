#include "text/Page.hpp"

#include "Color.hpp"
#include "graphics/Image.hpp"
#include "logger/Logger.hpp"

namespace gfx
{
namespace resources
{
//////////////////////////////////////////////////////////////////////
Page::Page(bool smooth) : nextRow(3)
{
    // Make sure that the texture is initialized by default
    gfx::resources::Image image;
    image.create({128, 128}, gfx::Color(255, 255, 255, 0));

    // Reserve a 2x2 white square for texturing underlines
    for (std::size_t i = 0; i < 2; ++i)
    {
        for (std::size_t j = 0; j < 2; ++j)
        {
            image.setPixel({static_cast<int>(i), static_cast<int>(j)}, gfx::Color(255, 255, 255, 255));
        }
    }

    // Create the texture
    if (not texture.loadFromImage(image))
    {
        LOG_ERROR << "Failed to load font page texture";
    }
    texture.setSmooth(smooth);
}

} // namespace resources
} // namespace gfx