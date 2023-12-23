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
    void draw(he::gfx::draw::IDrawable&, TransformMatrix&) override;
    void draw(he::gfx::draw::IDrawable&, const he::gfx::render::RenderSettings&, TransformMatrix&) override;
    void render() override;

public:
    void drawVertex2d(
        he::gfx::VertexArray2d& vertex,
        const unsigned int textureId,
        const he::gfx::Color color, 
        const he::gfx::render::RenderSettings& renderSettings,
        TransformMatrix&);
    void drawVertex3d(
        he::gfx::VertexArray3d& vertex,
        const unsigned int textureId,
        const he::gfx::Color color, 
        const he::gfx::render::RenderSettings& renderSettings,
        TransformMatrix&);

private:
    void draw2d(
        const he::gfx::VertexArray2d& vertex, 
        const unsigned int textureId,
        const he::gfx::Color color, 
        const he::gfx::render::RenderSettings& renderSettings,
        const bool isPrimitive = false) const;
    void draw3d(
        const he::gfx::VertexArray3d& vertex, 
        const unsigned int textureId,
        const he::gfx::Color color, 
        const he::gfx::render::RenderSettings& renderSettings,
        const bool isPrimitive = false) const;
    void viewportTransform(float& pX, float& pY, const float screenScale, const float offsetOriginScreen = -1.f);

private:
    std::unique_ptr<he::libs::gl::IGlWrapper> m_glWrapper{nullptr};
    he::gfx::render::RenderSettings m_defaultRenderSettings{};
    he::libs::gl::Shader m_texture2dShaderProgram;
    he::libs::gl::Shader m_primitive2dShaderProgram;
    he::libs::gl::Shader m_primitive3dShaderProgram;
    he::gfx::geometry::Size2Di m_windowSize;
    he::gfx::VertexArray2d m_cacheVertexArray{};
};
} // namespace render
} // namespace gfx
} // namespace he