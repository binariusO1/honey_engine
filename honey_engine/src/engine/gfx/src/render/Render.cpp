#include "gfx/render/Render.hpp"

#include "gfx/geometry/transform/Transform.hpp"
#include "gfx/draw/IDrawable.hpp"
#include "gfx/draw/Shape.hpp"
#include "gfx/draw/Sprite.hpp"
#include <iomanip>
#include "libs/gl/GlWrapper.hpp"
#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace render
{
////////////////////////////////////////////////////////////
Render::Render(const int screenWidth, const int screenHeight)
    : m_glWrapper{std::make_unique<he::libs::gl::GlWrapper>()}    
    , m_defaultRenderSettings{
        he::libs::gl::DrawType::Static,
        he::libs::gl::ConnectionType::TriangleFan,
        false
    }
    , m_primitive3dShaderProgram{"d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_3d.vs", "d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_3d.fs"}
    , m_primitive2dShaderProgram{"d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_primitive2d.vs", "d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_primitive2d.fs"}
    , m_texture2dShaderProgram{"d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_2d.vs", "d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_2d.fs"}
    , m_windowSize{screenWidth, screenHeight}
{
}


////////////////////////////////////////////////////////////
void Render::draw(he::gfx::draw::IDrawable& drawable, TransformMatrix& transform)
{
    draw(drawable, m_defaultRenderSettings, transform);
}


////////////////////////////////////////////////////////////
void Render::draw(he::gfx::draw::IDrawable& drawable, const he::gfx::render::RenderSettings& renderSettings, TransformMatrix& transform)
{
    drawable.draw(*this, renderSettings, transform);
}


////////////////////////////////////////////////////////////
void Render::render()
{
}


////////////////////////////////////////////////////////////
void Render::drawVertex2d(
        he::gfx::VertexArray2d& vertexArray,
        const unsigned int textureId,
        const he::gfx::Color color, 
        const he::gfx::render::RenderSettings& renderSettings,
        TransformMatrix& transformMatrix)
{
    if (transformMatrix.isNeedUpdate)
    {
        for (std::size_t i = 0 ; i < vertexArray.size() ; ++i)
        {
            vertexArray[i].position = transformMatrix.projectionTransform*transformMatrix.viewTransform*transformMatrix.modelTransform*vertexArray[i].position;
            // LOG_DEBUG << "Model point [" << i << "]: " << vertexArray[i].position;
            viewportTransform(vertexArray[i].position.x, vertexArray[i].position.y, 2.f);
        }
    }

    draw2d(vertexArray, textureId, color, renderSettings);
    transformMatrix.isNeedUpdate = false;
}


////////////////////////////////////////////////////////////
void Render::drawVertex3d(
        he::gfx::VertexArray3d& vertexArray,
        const unsigned int textureId,
        const he::gfx::Color color, 
        const he::gfx::render::RenderSettings& renderSettings,
        TransformMatrix& transformMatrix)
{
    if (transformMatrix.isNeedUpdate)
    {
        for (std::size_t i = 0 ; i < vertexArray.size() ; ++i)
        {
            vertexArray[i].position = transformMatrix.projectionTransform*transformMatrix.viewTransform*transformMatrix.modelTransform*vertexArray[i].position;
            // LOG_DEBUG << "Model point [" << i << "]: " << vertexArray[i].position;
            viewportTransform(vertexArray[i].position.x, vertexArray[i].position.y, 1.f, 0.f);
        }
        // LOG_DEBUG << "Model matrix: " << math::toString(transformMatrix.modelMatrix);
    }

    draw3d(vertexArray, textureId, color, renderSettings);
    transformMatrix.isNeedUpdate = false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
void Render::draw2d(
    const he::gfx::VertexArray2d& vertexArray, 
    const unsigned int textureId,
    const he::gfx::Color color, 
    const he::gfx::render::RenderSettings& renderSettings) const
{
    auto isPrimitive = not static_cast<bool>(textureId);
    auto size = sizeof(he::gfx::Vertex2d);
    const void* dataPointer = static_cast<const void*>(vertexArray.data());
    m_glWrapper->setupDraw(size, vertexArray.size(), dataPointer, renderSettings.drawType);
    std::uint8_t colorArray[4] = {color.r, color.g, color.b, color.a};

    if (isPrimitive)
    {
        m_glWrapper->drawPrimitive(0, vertexArray.size(), colorArray, textureId, renderSettings.prymitiveType, m_primitive2dShaderProgram.getId(), renderSettings.polygonMode);
    }
    else
    {
        m_glWrapper->draw(0, vertexArray.size(), colorArray, textureId, renderSettings.prymitiveType, m_texture2dShaderProgram.getId(), renderSettings.polygonMode);
    }
}


////////////////////////////////////////////////////////////
void Render::draw3d(
    const he::gfx::VertexArray3d& vertexArray, 
    const unsigned int textureId,
    const he::gfx::Color color, 
    const he::gfx::render::RenderSettings& renderSettings) const
{
    auto isPrimitive = not static_cast<bool>(textureId);
    auto size = sizeof(he::gfx::Vertex3d);
    const void* dataPointer = static_cast<const void*>(vertexArray.data());
    m_glWrapper->setupDraw(size, vertexArray.size(), dataPointer, renderSettings.drawType);
    std::uint8_t colorArray[4] = {color.r, color.g, color.b, color.a};

    if (isPrimitive)
    {
        // todo : change to 3d
        m_glWrapper->drawPrimitive(0, vertexArray.size(), colorArray, textureId, renderSettings.prymitiveType, m_primitive2dShaderProgram.getId(), renderSettings.polygonMode);
    }
    else
    {
        // todo : change to 3d
        m_glWrapper->draw(0, vertexArray.size(), colorArray, textureId, renderSettings.prymitiveType, m_texture2dShaderProgram.getId(), renderSettings.polygonMode);
    }
}


////////////////////////////////////////////////////////////
void Render::viewportTransform(float& pX, float& pY, const float screenScale, const float offsetOriginScreen)
{
    if (m_windowSize.width > 0 and m_windowSize.height > 0)
    {
        float x = (pX / static_cast<float>(m_windowSize.width)*screenScale) + offsetOriginScreen;
        float y = (pY / static_cast<float>(m_windowSize.height)*screenScale) + offsetOriginScreen;
        pX = x;
        pY = y;
    }
}
} // namespace render
} // namespace gfx
} // namespace he