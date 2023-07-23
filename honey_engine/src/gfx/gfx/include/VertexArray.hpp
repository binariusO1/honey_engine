#pragma once

#include <vector>
#include "Vertex.hpp"
#include "ColorBasic.hpp"

namespace gfx
{
using Vertex3dArray = std::vector<gfx::Vertex3d>;
using Vertex2dArray = std::vector<gfx::Vertex2d>;
using Vertex2dPrimitiveArray = std::vector<gfx::Vertex2dPrimitive>;
} // namespace gfx