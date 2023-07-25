#include "gfx/render/Render.hpp"

#include "libs/gl/GlWrapper.hpp"
#include "gfx/draw/IDrawable.hpp"
#include "gfx/draw/Shape.hpp"
#include "gfx/draw/Sprite.hpp"
#include "logger/Logger.hpp"
#include <iomanip>

namespace he
{
namespace gfx
{
namespace render
{
////////////////////////////////////////////////////////////
Render::Render()
    : m_glWrapper{std::make_unique<he::libs::gl::GlWrapper>()} 
    , m_renderSettings{
        he::libs::gl::DrawType::Static,
        he::libs::gl::ConnectionType::TriangleFan,
        false,
        false
    }
    , m_primitiveShaderProgram{"d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_primitive2d.vs", "d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_primitive2d.fs"}
    , m_textureShaderProgram{"d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_2d.vs", "d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_2d.fs"}
{
}


////////////////////////////////////////////////////////////
void Render::setRenderSettings(const he::gfx::render::RenderSettings& renderSettings)
{
    m_renderSettings = renderSettings;
}


////////////////////////////////////////////////////////////
void Render::draw(he::gfx::draw::IDrawable& drawable)
{
    draw(drawable, m_renderSettings);
}


////////////////////////////////////////////////////////////
void Render::draw(he::gfx::draw::IDrawable& drawable, he::gfx::render::RenderSettings& renderSettings)
{
    drawable.draw(*this, renderSettings);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
void Render::drawVertex(
    const he::gfx::VertexArray2d& vertexArray, 
    const unsigned int textureId,
    const he::gfx::Color color, 
    const he::gfx::render::RenderSettings& renderSettings)
{
    int shaderId = 0;
    if (renderSettings.isPrimitive)
    {
        shaderId = m_primitiveShaderProgram.getId();
    }
    else
    {
        shaderId = m_textureShaderProgram.getId();
    }
    auto size = sizeof(he::gfx::Vertex2d);
    const void* dataPointer = static_cast<const void*>(vertexArray.data());
    m_glWrapper->setupDraw(size, vertexArray.size(), dataPointer, renderSettings.drawType);
    std::uint8_t colorArray[4] = {color.r, color.g, color.b, color.a};
    m_glWrapper->draw(0, vertexArray.size(), colorArray, textureId, renderSettings.prymitiveType, shaderId, renderSettings.polygonMode, renderSettings.isPrimitive);
}
} // namespace render
} // namespace gfx
} // namespace he