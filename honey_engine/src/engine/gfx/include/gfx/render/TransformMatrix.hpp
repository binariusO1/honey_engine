#pragma once

namespace he
{
namespace gfx
{
namespace render
{
struct TransformMatrix
{
    const float* modelMatrix{nullptr};
    const float* projectionMatrix{nullptr};
    const float* viewMatrix{nullptr};
    bool isNeedUpdate{false};
}; 
} // namespace render
} // namespace gfx
} // namespace he