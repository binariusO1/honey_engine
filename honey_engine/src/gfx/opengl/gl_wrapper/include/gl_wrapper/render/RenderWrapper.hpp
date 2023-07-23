#pragma once

#include <cstddef>
#include "gl_wrapper/Source.hpp"
#include "gl_wrapper/render/PrimitiveType.hpp"
#include "gl_wrapper/render/DrawType.hpp"
#include "Vertex.hpp"
// #include "gfx/RenderSettings.hpp"
#include "gl_wrapper/render/Shader.hpp"

// TODO : ALL HERE IS TO DEVELOP

namespace gl_wrapper
{
class RenderWrapper
{
public:
    RenderWrapper();
    ~RenderWrapper();

    void draw(PrimitiveType type, std::size_t firstVertex, std::size_t vertexCount);
    void draw2dTexture(std::size_t firstVertex, std::size_t vertexCount, const gfx::Vertex2d* vertex, const int textureId, const gfx::render::RenderSettings& renderSettings);
    void draw2d(std::size_t firstVertex, std::size_t vertexCount, const gfx::Vertex2d* vertex, const gfx::render::RenderSettings& renderSettings);
    void draw2dPrimitives(std::size_t firstVertex, std::size_t vertexCount, const gfx::Vertex2dPrimitive* vertex, const gfx::Color color, const gfx::render::RenderSettings& renderSettings);

public:
    void flashColor();
    void terminate();

private:
    void init();
    void clear(const int&);
    void setupDraw(const gfx::Vertex2dPrimitive* vertex, std::size_t vertexCount, const gl_wrapper::DrawType drawType);
    void setupDraw2d(const gfx::Vertex2d* vertex, std::size_t vertexCount, const gl_wrapper::DrawType drawType);
    void setupDraw2dPrimitives(const gfx::Vertex2dPrimitive* vertex, std::size_t vertexCount, const gl_wrapper::DrawType drawType);

private:
    unsigned int vertexShader{};
    unsigned int fragmentShader{};
    unsigned int shaderProgram{};
    unsigned int VBO{}; // vertex buffer objects
    unsigned int VAO{};
    unsigned int EBO{}; // Element Buffer Objects
};
} // namespace gl_wrapper