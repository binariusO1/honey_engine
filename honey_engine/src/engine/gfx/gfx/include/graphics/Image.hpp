#pragma once

#include <filesystem>
#include <memory>
#include <vector>
#include "geometry/Point2d.hpp"
#include "geometry/Size2d.hpp"

namespace gfx
{
struct Color;
namespace resources
{

class Image
{
public:
    Image();
    ~Image();

public:
    void create(const geometry::Size2Di&, const gfx::Color&);

public:
    void setPixel(const geometry::Point2Di& coords, const gfx::Color& color);
    bool loadFromFile(const std::filesystem::path& filename);
    bool loadFromFileRaw(const std::filesystem::path& filename);
    const std::uint8_t* getPixelsPtr() const;
    geometry::Size2D getSize() const;

private:
    std::unique_ptr<unsigned char*> m_ptr;
    geometry::Size2d<int>           m_size;
    std::vector<std::uint8_t>       m_pixels;
};
} // namespace resources
} // namespace gfx