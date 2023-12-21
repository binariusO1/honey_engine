#pragma once

namespace he
{
namespace gfx
{
namespace render
{
struct TransformMatrix
{
    // TODO:
    // 1_ dorobic funkcje typu setModelMatrix() gdzie ustawiany jest z automatu bool isNeedUpdate
    // 2_ const float* przerobić na Transform (zastanowić sie)
    const float* modelMatrix{nullptr};
    const float* viewMatrix{nullptr};
    const float* projectionMatrix{nullptr};
    bool isNeedUpdate{false};
}; 
} // namespace render
} // namespace gfx
} // namespace he