#pragma once

namespace he
{
namespace gfx
{
namespace render
{
struct TransformSettings
{
    float* modelMatrix{nullptr};
    float* viewMatrix{nullptr};
    float* projectionMatrix{nullptr};
    bool isNeedUpdate{false};
}; 
} // namespace render
} // namespace gfx
} // namespace he