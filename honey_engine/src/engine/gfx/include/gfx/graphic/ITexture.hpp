#pragma once

#include "gfx/geometry/Size2d.hpp"

namespace he
{
namespace gfx
{
namespace render
{
class ITexture
{
public:
    virtual ~ITexture() = default;

public:
    virtual bool create(const geometry::Size2Dpxl& size, const std::uint8_t* pixels) = 0;
    virtual bool createEmpty(const he::gfx::geometry::Size2Dpxl& size) = 0;
    virtual void setSmooth(bool) = 0;
    virtual const he::gfx::geometry::Size2Dpxl getSize() const = 0;
    virtual const unsigned int getTextureId() const = 0;

    struct Context
    {
        Context()
        {
        }
        he::gfx::geometry::Size2Dpxl size{0, 0};
        unsigned int textureId{0};
    };
};
} // namespace render
} // namespace gfx
} // namespace he