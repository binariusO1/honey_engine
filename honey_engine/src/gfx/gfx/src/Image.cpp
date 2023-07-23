#include "graphics/Image.hpp"

#include "Color.hpp"
#include "input_stream/ImageLoader.hpp"
#include "logger/Logger.hpp"

namespace gfx
{
namespace resources
{
////////////////////////////////////////////////////////////
Image::Image()
{
    LOG_MEMORY << LOG_CONSTRUCTOR;
}


////////////////////////////////////////////////////////////
Image::~Image()
{
    LOG_MEMORY << LOG_DESTRUCTOR;
}


////////////////////////////////////////////////////////////
void Image::create(const geometry::Size2Di& size, const gfx::Color& color)
{
    if (size.width and size.height)
    {
        // Create a new pixel buffer first for exception safety's sake
        std::vector<std::uint8_t> newPixels(static_cast<std::size_t>(size.width) * static_cast<std::size_t>(size.height) * 4);

        // Fill it with the specified color
        std::uint8_t* ptr = newPixels.data();
        std::uint8_t* end = ptr + newPixels.size();
        while (ptr < end)
        {
            *ptr++ = color.r;
            *ptr++ = color.g;
            *ptr++ = color.b;
            *ptr++ = color.a;
        }

        // Commit the new pixel buffer
        m_pixels.swap(newPixels);

        // Assign the new size
        m_size = size;
    }
    else
    {
        // Dump the pixel buffer
        std::vector<std::uint8_t>().swap(m_pixels);

        // Assign the new size
        m_size.width = 0;
        m_size.height = 0;
    }
}


////////////////////////////////////////////////////////////
bool Image::loadFromFile(const std::filesystem::path& filename)
{
    m_ptr = nullptr;
    return sys::input_stream::ImageLoader::getInstance().loadImageFromFile(filename, m_pixels, m_size);
}


////////////////////////////////////////////////////////////
bool Image::loadFromFileRaw(const std::filesystem::path& filename)
{
    m_pixels.clear();
    m_pixels.shrink_to_fit();
    m_ptr = std::make_unique<unsigned char*>();
    return sys::input_stream::ImageLoader::getInstance().loadImageFromFile(filename, *m_ptr.get(), m_size);
}


////////////////////////////////////////////////////////////
geometry::Size2D Image::getSize() const
{
    return geometry::Size2D({static_cast<std::uint16_t>(m_size.width), static_cast<std::uint16_t>(m_size.height)});
}


////////////////////////////////////////////////////////////
void Image::setPixel(const geometry::Point2Di& coords, const gfx::Color& color)
{
    std::uint8_t* pixel = &m_pixels[(coords.x + coords.y * m_size.width) * 4];
    *pixel++            = color.r;
    *pixel++            = color.g;
    *pixel++            = color.b;
    *pixel++            = color.a;
}


////////////////////////////////////////////////////////////
const std::uint8_t* Image::getPixelsPtr() const
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

} // namespace resources
} // namespace gfx