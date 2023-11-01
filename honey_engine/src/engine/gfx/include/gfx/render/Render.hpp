#pragma once

#include <memory>
#include "gfx/Color.hpp"
#include "gfx/Vertex.hpp"
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
    Render();
    ~Render() override = default;
    Render(const Render&) = delete;
    Render& operator=(const Render&) = delete;

public:
    void draw(const he::gfx::draw::IDrawable&) override;
    void draw(const he::gfx::draw::IDrawable&, const he::gfx::render::RenderSettings&) override;

public:
    void drawVertex(
        const he::gfx::VertexArray2d& vertex, 
        const unsigned int textureId,
        const he::gfx::Color color, 
        const he::gfx::render::RenderSettings& renderSettings) const;

    void drawVertexPrimitive(
        const he::gfx::VertexArray2d& vertex, 
        const unsigned int textureId,
        const he::gfx::Color color, 
        const he::gfx::render::RenderSettings& renderSettings) const;

private:
    void drawVertexArray(
        const he::gfx::VertexArray2d& vertex, 
        const unsigned int textureId,
        const he::gfx::Color color, 
        const he::gfx::render::RenderSettings& renderSettings,
        const bool isPrimitive = false) const;

private:
    std::unique_ptr<he::libs::gl::IGlWrapper> m_glWrapper{nullptr};
    he::gfx::render::RenderSettings m_defaultRenderSettings{};
    he::libs::gl::Shader m_textureShaderProgram;
    he::libs::gl::Shader m_primitiveShaderProgram;
};
} // namespace render
} // namespace gfx
} // namespace he