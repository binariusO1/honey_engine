#pragma once

#include <cstdint>
#include "libs/gl/DrawType.hpp"
#include "libs/gl/ConnectionType.hpp"
#include "libs/gl/IGlWrapper.hpp"

namespace he
{
namespace libs
{
namespace gl
{

class GlWrapper : public IGlWrapper
{
public:
    GlWrapper() = default;
    ~GlWrapper() override = default;

public:
    void setupDraw(const GLsizeiptr size, std::size_t vertexCount, const void* data, const DrawType drawType) override;
    void draw(std::size_t firstVertex, std::size_t vertexCount, const uint8_t* colorArray, const int textureId, he::libs::gl::ConnectionType prymitiveType, int shaderId, bool polygonMode) override;
    void drawPrimitive(std::size_t firstVertex, std::size_t vertexCount, const uint8_t* colorArray, const int textureId, he::libs::gl::ConnectionType prymitiveType, int shaderId, bool polygonMode) override;

public:
    void getIntegerv(const unsigned int pname, int* data) const override;
    void bindTexture(const unsigned int target, const unsigned int texture) const override;
    void flush() const override;
    void generateTextureId(unsigned int& textureId) const override;
    void specifyTextureImage2D(const unsigned int target, 
                                        const int level, 
                                        const int internalformat,
                                        const unsigned int width, 
                                        const unsigned int height, 
                                        const int border, 
                                        const unsigned int format, 
                                        const unsigned int type, 
                                        const void * data) const override;
    void specifyTextureSubImage2D(const unsigned int target, 
                                        const int level, 
                                        const int xoffset, 
                                        const int yoffset, 
                                        const int width, 
                                        const int height, 
                                        const unsigned int format, 
                                        const unsigned int type, 
                                        const void * pixels) const override;
    void setTextureParameters(const unsigned int target, const unsigned int pname, const int param) const override;
    void generateMipMap(const unsigned int target) const override;
    void deleteTexture(const unsigned int textureId) const override;

    void clear(const int& programShaderId) const override;
private:

    unsigned int VBO{}; // vertex buffer objects
    unsigned int VAO{};
    unsigned int EBO{}; // Element Buffer Objects
};
} // namespace gl
} // namespace libs
} // namespace he