#pragma once


#include <filesystem>
#include <vector>
#include "gfx/geometry/Size2d.hpp"

namespace he
{
namespace gfx
{
struct Color;
namespace render
{

class Image
{
public:
    Image(const he::gfx::geometry::Size2Dpxl&, const he::gfx::Color&);
    Image(const std::filesystem::path&);
    ~Image();

public:
    geometry::Size2D getSize() const;
    void setPixel(const he::gfx::geometry::Size2Dpxl& coords, const gfx::Color& color);
    const std::uint8_t* getData() const;

private:
    bool create(const he::gfx::geometry::Size2Dpxl&, const he::gfx::Color&);

    geometry::Size2Dpxl m_size;
    std::vector<std::uint8_t> m_pixels;
};
} // namespace render
} // namespace gfx
} // namespace he