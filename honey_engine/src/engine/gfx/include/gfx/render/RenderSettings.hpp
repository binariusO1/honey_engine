#pragma once

#include "libs/gl/DrawType.hpp"
#include "libs/gl/ConnectionType.hpp"

namespace he
{
namespace gfx
{
namespace render
{
struct RenderSettings
{
    he::libs::gl::DrawType drawType{he::libs::gl::DrawType::Static};
    he::libs::gl::ConnectionType prymitiveType{he::libs::gl::ConnectionType::TriangleFan};
    bool polygonMode{false};
}; 
} // namespace render
} // namespace gfx
} // namespace he