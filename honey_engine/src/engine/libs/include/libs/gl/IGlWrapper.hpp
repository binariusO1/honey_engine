#pragma once

#include <libs/glad/glad.h> // note: it must be higher than other gl libraries
#include <cstdint>
#include "libs/gl/DrawType.hpp"
#include "libs/gl/ConnectionType.hpp"

namespace he
{
namespace libs
{
namespace gl
{
class IGlWrapper 
{
public:
    virtual ~IGlWrapper() = default;

public:
    virtual void setupDraw(const GLsizeiptr size, std::size_t vertexCount, const void* data, const DrawType drawType) = 0;
    virtual void draw(std::size_t firstVertex, std::size_t vertexCount, const uint8_t* colorAlpha, const int textureId, he::libs::gl::ConnectionType prymitiveType, int shaderId, bool polygonMode) = 0;
    virtual void drawPrimitive(std::size_t firstVertex, std::size_t vertexCount, const uint8_t* colorAlpha, const int textureId, he::libs::gl::ConnectionType prymitiveType, int shaderId, bool polygonMode) = 0;

public:
    virtual void getIntegerv(const unsigned int pname, int* data) const = 0;
    virtual void bindTexture(const unsigned int target, const unsigned int texture) const = 0;
    virtual void flush() const = 0;
    virtual void generateTextureId(unsigned int& textureId) const = 0;
    virtual void specifyTextureImage2D(const unsigned int target, 
                                        const int level, 
                                        const int internalformat,
                                        const unsigned int width, 
                                        const unsigned int height, 
                                        const int border, 
                                        const unsigned int format, 
                                        const unsigned int type, 
                                        const void * data) const = 0;
    virtual void specifyTextureSubImage2D(const unsigned int target, 
                                        const int level, 
                                        const int xoffset, 
                                        const int yoffset, 
                                        const int width, 
                                        const int height, 
                                        const unsigned int format, 
                                        const unsigned int type, 
                                        const void * pixels) const = 0;
    virtual void setTextureParameters(const unsigned int target, const unsigned int pname, const int param) const = 0;
    virtual void generateMipMap(const unsigned int target) const = 0;
    virtual void deleteTexture(const unsigned int textureId) const = 0;
    virtual void clear(const int& programShaderId) const = 0;
};
} // namespace gl
} // namespace libs
} // namespace he