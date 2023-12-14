#pragma once

#include <memory>
#include "gfx/Color.hpp"
#include "gfx/Vertex.hpp"
#include "gfx/geometry/Size2d.hpp"
#include "gfx/render/IRender.hpp"
#include "gfx/render/RenderSettings.hpp"
#include "libs/gl/IGlWrapper.hpp"
#include "libs/gl/Shader.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
class IDrawable;
} // namespace draw
namespace render
{

class Render : public IRender
{
public:
    Render(const int screenWidth, const int screenHeight);
    ~Render() override = default;
    Render(const Render&) = delete;
    Render& operator=(const Render&) = delete;

public:
    void draw(he::gfx::draw::IDrawable&) override;
    void draw(he::gfx::draw::IDrawable&, const he::gfx::render::RenderSettings&) override;

public:
    void drawVertex2d(
        he::gfx::VertexArray2d& vertex,
        const unsigned int textureId,
        const he::gfx::Color color, 
        const he::gfx::render::RenderSettings& renderSettings,
        const float* matrix,
        bool needUpdate);

private:
    void drawVertexArray(
        const he::gfx::VertexArray2d& vertex, 
        const unsigned int textureId,
        const he::gfx::Color color, 
        const he::gfx::render::RenderSettings& renderSettings,
        const bool isPrimitive = false) const;//TODO : change method name to 2D
    void viewportTransform(he::gfx::geometry::Point2Df& point);

private:
    std::unique_ptr<he::libs::gl::IGlWrapper> m_glWrapper{nullptr};
    he::gfx::render::RenderSettings m_defaultRenderSettings{};
    he::libs::gl::Shader m_textureShaderProgram;
    he::libs::gl::Shader m_primitiveShaderProgram;
    he::gfx::geometry::Size2Di m_windowSize;
    he::gfx::VertexArray2d m_cacheVertexArray{};
};
} // namespace render
} // namespace gfx
} // namespace he