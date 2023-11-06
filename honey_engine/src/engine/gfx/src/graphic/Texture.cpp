#include "gfx/graphic/Texture.hpp"

#include <cassert>
#include "exception/invalid_initialization.hpp"
#include "gfx/Color.hpp"
#include "gfx/graphic/Image.hpp"
#include "libs/gl/GlWrapper.hpp"
#include "logger/Logger.hpp"

namespace
{
} // namespace

namespace he
{
namespace gfx
{
namespace render
{
////////////////////////////////////////////////////////////
Texture::Texture() : m_glWrapper{std::make_shared<he::libs::gl::GlWrapper>()}
{
}


////////////////////////////////////////////////////////////
Texture::Texture(const std::string& filepath) : m_glWrapper{std::make_shared<he::libs::gl::GlWrapper>()}
{
    Image image(filepath);
    if(not create(image.getSize(), image.getData()))
    {
        throw he::common::invalid_initialization("Unable to create Texture");
    } 
}


////////////////////////////////////////////////////////////
Texture::Texture(const he::gfx::render::Image image) : m_glWrapper{std::make_shared<he::libs::gl::GlWrapper>()}
{
    if(not create(image.getSize(), image.getData()))
    {
        throw he::common::invalid_initialization("Unable to create Texture");
    } 
}


////////////////////////////////////////////////////////////
Texture::~Texture()
{
    if (m_context.textureId)
    {
        m_glWrapper->deleteTexture(m_context.textureId);
    }
}
////////////////////////////////////////////////////////////
void Texture::setSmooth(bool smooth)
{
    if (m_context.textureId)
    {
        GLint textureBinding = 0;
        m_glWrapper->getIntegerv(GL_TEXTURE_BINDING_2D, &textureBinding);
        m_glWrapper->bindTexture(GL_TEXTURE_2D, m_context.textureId);
        m_glWrapper->setTextureParameters(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR : GL_NEAREST);

        bool m_hasMipMap = true;
        if (m_hasMipMap)
        {
            m_glWrapper->setTextureParameters(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);
        }
        else
        {
            m_glWrapper->setTextureParameters(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                smooth ? GL_LINEAR : GL_NEAREST);
        }
    }
    else
    {
        LOG_WARNING << "Cannot set texture smooth, because texture is not initialized";
    }
}


////////////////////////////////////////////////////////////
bool Texture::create(const geometry::Size2Dpxl& size, const std::uint8_t* data)
{
    if (data and prepareTexture(size))
    {
        createTexture(size, data);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////
bool Texture::create(const geometry::Size2Dpxl& size)
{
    if ((size.width == 0) or (size.height == 0))
    {
        LOG_WARNING << "Failed to create texture, invalid size (" << size.width << "x" << size.height << ")";
        return false;
    }

    GLint sizeToCheck = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &sizeToCheck);
    unsigned int maxSize = static_cast<unsigned int>(sizeToCheck);

    if ((size.width > maxSize) or (size.height > maxSize))
    { 
        LOG_ERROR << "Failed to create texture, its internal size is too high "
              << "(" << size.width << "x" << size.height << ", "
              << "maximum is " << maxSize << "x" << maxSize << ")" << std::endl;
        return false;
    }

    m_context.size = size;

    if (not m_context.textureId)
    {
        GLuint texture;
        glGenTextures(1, &texture);
        m_context.textureId = texture;
    }
    LOG_DEBUG << "\x1B[32mCreate texture with id: " << m_context.textureId << "\033[0m";
    bool textureEdgeClamp = false;

    glBindTexture(GL_TEXTURE_2D, m_context.textureId); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(size.width), static_cast<GLsizei>(size.height), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_isRepeated ? GL_REPEAT : (textureEdgeClamp ? GL_CLAMP_TO_EDGE : GL_CLAMP));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_isRepeated ? GL_REPEAT : (textureEdgeClamp ? GL_CLAMP_TO_EDGE : GL_CLAMP));
    glGenerateMipmap(GL_TEXTURE_2D);
    m_hasMipmap = false;

    return true;
}


////////////////////////////////////////////////////////////
bool Texture::createEmpty(const geometry::Size2Dpxl& size)
{
    if (prepareTexture(size))
    {
        Image image(size, he::gfx::Color::White);
        createTexture(size, image.getData());
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////
const he::gfx::geometry::Size2Dpxl Texture::getSize() const
{
    return m_context.size;
}


////////////////////////////////////////////////////////////
const unsigned int Texture::getTextureId() const
{
    return m_context.textureId;
}


////////////////////////////////////////////////////////////
const unsigned int Texture::getMaxTextureSize()
{
    GLint sizeToCheck = 0;
    m_glWrapper->getIntegerv(GL_MAX_TEXTURE_SIZE, &sizeToCheck);
    return sizeToCheck;
}


////////////////////////////////////////////////////////////
void Texture::update(const Texture& texture)
{
    update(texture, {0, 0});
}


////////////////////////////////////////////////////////////
void Texture::update(const Texture& texture, const geometry::Vector2Du& dest)
{
    if (not texture.m_context.textureId)
    {
        return;
    }

    {
        GLint readFramebuffer = 0;
        GLint drawFramebuffer = 0;
        glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFramebuffer);
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebuffer);

        // Create the framebuffers
        GLuint sourceFrameBuffer = 0;
        GLuint destFrameBuffer   = 0;
        glGenFramebuffers(1, &sourceFrameBuffer);
        glGenFramebuffers(1, &destFrameBuffer);

        if (not sourceFrameBuffer or not destFrameBuffer)
        {
            LOG_ERROR << "Cannot copy texture, failed to create a frame buffer object";
            return;
        }

        // Link the source texture to the source frame buffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceFrameBuffer);
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER,
                                             GL_COLOR_ATTACHMENT0,
                                             GL_TEXTURE_2D,
                                             texture.m_context.textureId,
                                             0);

        // Link the destination texture to the destination frame buffer
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFrameBuffer);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_context.textureId, 0);

        // A final check, just to be sure...
        GLenum sourceStatus;
        sourceStatus = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);

        GLenum destStatus;
        destStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

        if ((sourceStatus == GL_FRAMEBUFFER_COMPLETE) and (destStatus == GL_FRAMEBUFFER_COMPLETE))
        {
            // Blit the texture contents from the source to the destination texture
            glBlitFramebuffer(0,
                    texture.m_pixelsFlipped ? static_cast<GLint>(texture.m_context.size.height) : 0,
                    static_cast<GLint>(texture.m_context.size.width),
                    texture.m_pixelsFlipped ? 0 : static_cast<GLint>(texture.m_context.size.height), // Source rectangle, flip y if source is flipped
                    static_cast<GLint>(dest.x),
                    static_cast<GLint>(dest.y),
                    static_cast<GLint>(dest.x + texture.m_context.size.width),
                    static_cast<GLint>(dest.y + texture.m_context.size.height), // Destination rectangle
                    GL_COLOR_BUFFER_BIT,
                    GL_NEAREST);
        }
        else
        {
            LOG_ERROR << "Cannot copy texture, failed to link texture to frame buffer";
        }

        // Restore previously bound framebuffers
        glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(readFramebuffer));
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, static_cast<GLuint>(drawFramebuffer));

        // Delete the framebuffers
        glDeleteFramebuffers(1, &sourceFrameBuffer);
        glDeleteFramebuffers(1, &destFrameBuffer);

        GLint textureBinding;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureBinding);

        // Set the parameters of this texture
        glBindTexture(GL_TEXTURE_2D, m_context.textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST);
        m_hasMipmap     = false;
        m_pixelsFlipped = false;

        // Force an OpenGL flush, so that the texture data will appear updated
        // in all contexts immediately (solves problems in multi-threaded apps)
        glFlush();
        return;
    }

    // TODO
    LOG_WARNING << "no implemented";
}



////////////////////////////////////////////////////////////
void Texture::swap(Texture& right)
{
    std::swap(m_context.size, right.m_context.size);
    //  std::swap(m_actualSize, right.m_actualSize);
    std::swap(m_context.textureId, right.m_context.textureId);
    std::swap(m_isSmooth, right.m_isSmooth);
    // std::swap(m_sRgb, right.m_sRgb);
    // std::swap(m_isRepeated, right.m_isRepeated);
    std::swap(m_pixelsFlipped, right.m_pixelsFlipped);
    // std::swap(m_fboAttachment, right.m_fboAttachment);
    std::swap(m_hasMipmap, right.m_hasMipmap);

    auto id = right.getTextureId();
    right.m_context.textureId = m_context.textureId;
    m_context.textureId = id;
}


////////////////////////////////////////////////////////////
void Texture::update(const std::uint8_t* pixels, const geometry::Size2Dui& size, const geometry::Vector2Dui& destination)
{
    if ( not (destination.x + size.width <= m_context.size.width) and not (destination.y + size.height <= m_context.size.height))
    {
        LOG_WARNING << "Cannot update texture!";
        return;
    }

    if (pixels and m_context.textureId)
    {
        // Make sure that the current texture binding will be preserved
        GLint textureBinding;
        m_glWrapper->getIntegerv(GL_TEXTURE_BINDING_2D, &textureBinding);

        // Copy pixels from the given array to the texture
        m_glWrapper->bindTexture(GL_TEXTURE_2D, m_context.textureId);
        m_glWrapper->specifyTextureSubImage2D(GL_TEXTURE_2D,
                                        0,
                                        static_cast<GLint>(destination.x),
                                        static_cast<GLint>(destination.y),
                                        static_cast<GLsizei>(size.width),
                                        static_cast<GLsizei>(size.height),
                                        GL_RGBA,
                                        GL_UNSIGNED_BYTE,
                                        pixels);

        m_glWrapper->setTextureParameters(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST);
        m_hasMipmap = false;
        m_pixelsFlipped = false;

        // Force an OpenGL flush, so that the texture data will appear updated
        // in all contexts immediately (solves problems in multi-threaded apps)
        m_glWrapper->flush();
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
bool Texture::prepareTexture(const geometry::Size2Dpxl& size)
{
    if ((size.width == 0) or (size.height == 0))
    {
        LOG_WARNING << "Invalid size: [" << size.width << "x" << size.height << "]";
        return false;
    }
    auto sizeToCheck = getMaxTextureSize();

    if ((size.width > sizeToCheck) or (size.height > sizeToCheck))
    { 
        LOG_ERROR << "Failed to create texture, its internal size is too high "
              << "[" << size.width << "x" << size.height << "]. "
              << "Maximum is [" << sizeToCheck << "x" << sizeToCheck << "]";
        return false;
    }

    m_context.size = size;

    if (not m_context.textureId)
    {
        m_glWrapper->generateTextureId(m_context.textureId);
    }
    return true;
}


////////////////////////////////////////////////////////////
void Texture::createTexture(const geometry::Size2Dpxl& size, const std::uint8_t* data)
{
        m_glWrapper->bindTexture(GL_TEXTURE_2D, m_context.textureId);
        m_glWrapper->specifyTextureImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_context.size.width, m_context.size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        m_glWrapper->setTextureParameters(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        m_glWrapper->setTextureParameters(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        m_glWrapper->setTextureParameters(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        m_glWrapper->setTextureParameters(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        m_glWrapper->generateMipMap(GL_TEXTURE_2D);
        m_glWrapper->flush();
        LOG_DEBUG << "Create texture id: " << m_context.textureId << ", size: [" << m_context.size.width << "x" << m_context.size.height << "]";
}


} // namespace render
} // namespace gfx
} // namespace he