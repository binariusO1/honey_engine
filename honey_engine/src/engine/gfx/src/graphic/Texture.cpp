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