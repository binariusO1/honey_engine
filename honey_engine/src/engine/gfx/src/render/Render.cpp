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
    , m_defaultRenderSettings{
        he::libs::gl::DrawType::Static,
        he::libs::gl::ConnectionType::TriangleFan,
        false
    }
    , m_primitiveShaderProgram{"d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_primitive2d.vs", "d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_primitive2d.fs"}
    , m_textureShaderProgram{"d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_2d.vs", "d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_2d.fs"}
{
}


////////////////////////////////////////////////////////////
void Render::draw(const he::gfx::draw::IDrawable& drawable)
{
    draw(drawable, m_defaultRenderSettings);
}


////////////////////////////////////////////////////////////
void Render::draw(const he::gfx::draw::IDrawable& drawable, const he::gfx::render::RenderSettings& renderSettings)
{
    drawable.draw(*this, renderSettings);
}


////////////////////////////////////////////////////////////
void Render::drawVertex(
    const he::gfx::VertexArray2d& vertexArray, 
    const unsigned int textureId,
    const he::gfx::Color color, 
    const he::gfx::render::RenderSettings& renderSettings) const
{

    drawVertexArray(vertexArray, textureId, color, renderSettings);
}


////////////////////////////////////////////////////////////
void Render::drawVertexPrimitive(
    const he::gfx::VertexArray2d& vertexArray, 
    const unsigned int textureId,
    const he::gfx::Color color, 
    const he::gfx::render::RenderSettings& renderSettings) const
{
    drawVertexArray(vertexArray, textureId, color, renderSettings, true);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
void Render::drawVertexArray(
    const he::gfx::VertexArray2d& vertexArray, 
    const unsigned int textureId,
    const he::gfx::Color color, 
    const he::gfx::render::RenderSettings& renderSettings,
    const bool isPrimitive) const
{
    auto size = sizeof(he::gfx::Vertex2d);
    const void* dataPointer = static_cast<const void*>(vertexArray.data());
    m_glWrapper->setupDraw(size, vertexArray.size(), dataPointer, renderSettings.drawType);
    std::uint8_t colorArray[4] = {color.r, color.g, color.b, color.a};

    if (isPrimitive)
    {
        m_glWrapper->drawPrimitive(0, vertexArray.size(), colorArray, textureId, renderSettings.prymitiveType, m_primitiveShaderProgram.getId(), renderSettings.polygonMode);
    }
    else
    {
        m_glWrapper->draw(0, vertexArray.size(), colorArray, textureId, renderSettings.prymitiveType, m_textureShaderProgram.getId(), renderSettings.polygonMode);
    }
}

} // namespace render
} // namespace gfx
} // namespace he