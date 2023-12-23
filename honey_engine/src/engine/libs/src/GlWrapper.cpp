#include "libs/gl/GlWrapper.hpp"
#include "logger/Logger.hpp"
namespace
{
float uint8toFloat(const std::uint8_t value)
{
    return static_cast<float>(value)/255.0f;
}
} // namespace
namespace he
{
namespace libs
{
namespace gl
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlWrapper::setupDraw(const GLsizeiptr size, std::size_t vertexCount, const void* data, const DrawType drawType)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); 
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, (vertexCount) * size, data, drawType);

    // note: vertex
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, size, (void*)0);
    glEnableVertexAttribArray(0);
    // note: color
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, size, (void*)(2* sizeof(float)));
    glEnableVertexAttribArray(1);
    // note: texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size, (void*)( (2* sizeof(float) ) + (4*sizeof(unsigned char) )));
    glEnableVertexAttribArray(2);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlWrapper::draw(std::size_t firstVertex, std::size_t vertexCount, const uint8_t* color, const int textureId, he::libs::gl::ConnectionType connectionType, int shaderId, bool polygonMode)
{
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUseProgram(shaderId);
    if (color[4] < 255)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    glBindVertexArray(VAO);
    glDrawArrays(connectionType, static_cast<GLint>(firstVertex), static_cast<GLsizei>(vertexCount));

    if (polygonMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    // clear(shaderId);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlWrapper::drawPrimitive(std::size_t firstVertex, std::size_t vertexCount, const uint8_t* color, const int textureId, he::libs::gl::ConnectionType connectionType, int shaderId, bool polygonMode)
{
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUseProgram(shaderId);
    if (color[4] < 255)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    int vertexColorLocation = glGetUniformLocation(shaderId, "PrimitivesColor");
    glUniform4f(vertexColorLocation, uint8toFloat(color[0]), uint8toFloat(color[1]), uint8toFloat(color[2]), uint8toFloat(color[3]));

    glBindVertexArray(VAO);
    glDrawArrays(connectionType, static_cast<GLint>(firstVertex), static_cast<GLsizei>(vertexCount));

    if (polygonMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glEnd();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    // glUseProgram(0);
    // clear(shaderId);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlWrapper::getIntegerv(const unsigned int pname, int* data) const
{
    glGetIntegerv(pname, data);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlWrapper::bindTexture(const unsigned int target, const unsigned int texture) const
{
    glBindTexture(target, texture);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlWrapper::flush() const
{
    glFlush();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlWrapper::generateTextureId(unsigned int& textureId) const
{
    glGenTextures(1, &textureId);
}

    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlWrapper::specifyTextureImage2D(const unsigned int target, 
                                    const int level, 
                                    const int internalformat,
                                    const unsigned int width, 
                                    const unsigned int height, 
                                    const int border, 
                                    const unsigned int format, 
                                    const unsigned int type, 
                                    const void * data) const
{
    glTexImage2D(target, level, internalformat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), border, format, type, data);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlWrapper::specifyTextureSubImage2D(const unsigned int target, 
                                        const int level, 
                                        const int xoffset, 
                                        const int yoffset, 
                                        const int width, 
                                        const int height, 
                                        const unsigned int format, 
                                        const unsigned int type, 
                                        const void * pixels) const
{
        glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlWrapper::setTextureParameters(const unsigned int target, const unsigned int pname, const int param) const
{
    glTexParameteri(target, pname, param);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlWrapper::generateMipMap(const unsigned int target) const
{
    glGenerateMipmap(target);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlWrapper::deleteTexture(const unsigned int textureId) const
{
    LOG_DEBUG << "Delete textureId: " << textureId;
    glDeleteTextures(1, &textureId);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GlWrapper::clear(const int& programShaderId) const
{
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




} // namespace gl
} // namespace libs
} // namespace he