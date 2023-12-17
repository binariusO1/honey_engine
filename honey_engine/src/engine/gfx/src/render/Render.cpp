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
    , m_primitiveShaderProgram{"d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_primitive2d.vs", "d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_primitive2d.fs"}
    , m_textureShaderProgram{"d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_2d.vs", "d:\\Projects\\CPP\\games\\honey_engine\\data\\gfx\\render\\shaders\\shader_2d.fs"}
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
            he::math::transformPoint2d(vertexArray[i].position.x, vertexArray[i].position.y, transformMatrix.projectionMatrix);
            he::math::transformPoint2d(vertexArray[i].position.x, vertexArray[i].position.y, transformMatrix.viewMatrix);
            he::math::transformPoint2d(vertexArray[i].position.x, vertexArray[i].position.y, transformMatrix.modelMatrix);
            viewportTransform(vertexArray[i].position);
        }
    }

    auto isPrimitive = not static_cast<bool>(textureId);
    draw2d(vertexArray, textureId, color, renderSettings, isPrimitive);
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


////////////////////////////////////////////////////////////
void Render::viewportTransform(he::gfx::geometry::Point2Df& point)
{
    const float SCREEN_SCALE = 2.0f;
    const float OFFSET_ORIGIN_SCREEN = -1.0f;

    if (m_windowSize.width > 0 and m_windowSize.height > 0)
    {
        float x = (point.x / static_cast<float>(m_windowSize.width)*SCREEN_SCALE) + OFFSET_ORIGIN_SCREEN;
        float y = (point.y / static_cast<float>(m_windowSize.height)*SCREEN_SCALE) + OFFSET_ORIGIN_SCREEN;
        point.x = x;
        point.y = y;
    }
}
} // namespace render
} // namespace gfx
} // namespace he