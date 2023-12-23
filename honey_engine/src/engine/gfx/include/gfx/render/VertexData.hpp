#pragma once

#include "gfx/Color.hpp"
#include "gfx/render/RenderSettings.hpp"

namespace he
{
namespace gfx
{
namespace render
{
struct VertexData
{
    const unsigned long long firstIndex{0};
    const unsigned long long lastIndex{0};
    const unsigned int textureId{0};
    const he::gfx::Color color{gfx::Color::White};
    const he::gfx::render::RenderSettings renderSettings{};
};
} // namespace render
} // namespace gfx
} // namespace he