#include "gl_wrapper/resources/GlResource.hpp"

namespace gl_wrapper
{
////////////////////////////////////////////////////////////
GlResource::GlResource() = default;


////////////////////////////////////////////////////////////

void GlResource::gl_DeleteTexture(const unsigned int textureId)
{
    GLuint texture = textureId;
    glDeleteTextures(1, &texture);
}


} // namespace gl_wrapper