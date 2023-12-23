#pragma once

#include "gfx/geometry/transform/Transform.hpp"

namespace he
{
namespace gfx
{
namespace render
{
struct TransformMatrix
{
    void setProjectionTransform(const geometry::transform::Transform& transform)
    {
        projectionTransform = transform;
        isNeedUpdate = true;
    }

    void setViewTransform(const geometry::transform::Transform& transform)
    {
        viewTransform = transform;
        isNeedUpdate = true;
    }

    void setModelTransform(const geometry::transform::Transform& transform)
    {
        viewTransform = transform;
        isNeedUpdate = true;
    }

    geometry::transform::Transform modelTransform{1.f};
    geometry::transform::Transform viewTransform{1.f};
    geometry::transform::Transform projectionTransform{1.f};
    bool isNeedUpdate{true};
}; 
} // namespace render
} // namespace gfx
} // namespace he