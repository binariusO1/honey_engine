#include "gfx/graphic/Image.hpp"

#include "exception/invalid_initialization.hpp"
#include "gfx/Color.hpp"
#include "logger/Logger.hpp"
#include "sys/ImageLoader.hpp"

namespace he
{
namespace gfx
{
namespace render
{

////////////////////////////////////////////////////////////
Image::Image(const he::gfx::geometry::Size2Dpxl& size, const he::gfx::Color& color)
{
    if(not create(size, color))
    {
        throw he::common::invalid_initialization("Unable to create Image");
    }
}


////////////////////////////////////////////////////////////
Image::Image(const std::filesystem::path& filename)
{
    int width, height = 0;
    m_pixels.clear();
    m_pixels.shrink_to_fit();
    auto result = he::sys::ImageLoader::getInstance().loadImageFromFile(filename, m_pixels, width, height);
    if (not result or width > UINT16_MAX or height > UINT16_MAX)
    {
        throw he::common::invalid_initialization("Unable to create Image");
    }
    m_size.width = static_cast<uint16_t>(width);
    m_size.height = static_cast<uint16_t>(height);
}


////////////////////////////////////////////////////////////
Image::~Image()
{
}


////////////////////////////////////////////////////////////
void Image::setPixel(const geometry::Size2Dpxl& coords, const gfx::Color& color)
{
    std::uint8_t* pixel = &m_pixels[(coords.width + coords.height * m_size.width) * 4];
    *pixel++            = color.r;
    *pixel++            = color.g;
    *pixel++            = color.b;
    *pixel++            = color.a;
}


////////////////////////////////////////////////////////////
geometry::Size2D Image::getSize() const
{
    return geometry::Size2D({m_size.width,m_size.height});
}


////////////////////////////////////////////////////////////
bool Image::create(const he::gfx::geometry::Size2Dpxl& size, const he::gfx::Color& color)
{
    if (size.width and size.height)
    {
        std::vector<std::uint8_t> newPixels(static_cast<std::size_t>(size.width) * static_cast<std::size_t>(size.height) * 4);
        std::uint8_t* ptr = newPixels.data();
        std::uint8_t* end = ptr + newPixels.size();
        while (ptr < end)
        {
            *ptr++ = color.r;
            *ptr++ = color.g;
            *ptr++ = color.b;
            *ptr++ = color.a;
        }
        m_pixels.swap(newPixels);
        m_size = size;
        return true;
    }
    std::vector<std::uint8_t>().swap(m_pixels);
    m_size.width = 0;
    m_size.height = 0;
    return false;
}


////////////////////////////////////////////////////////////
const std::uint8_t* Image::getData() const
{
    if (not m_pixels.empty())
    {
        return m_pixels.data();
    }
    else
    {
        LOG_ERROR << "Trying to access the pixels of an empty image";
        return nullptr;
    }
}


} // namespace render
} // namespace gfx
} // namespace he