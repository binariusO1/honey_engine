#include "graphics/Texture.hpp"

#include <cassert>
#include <mutex>
#include <vector>
#include "logger/Logger.hpp"
#include "input_stream/ImageLoader.hpp"
#include "gfx/Color.hpp"
#include "gfx/graphics/Image.hpp"

namespace
{

namespace TextureImpl
{
std::recursive_mutex idMutex;
std::recursive_mutex maximumSizeMutex;

// Thread-safe unique identifier generator,
// is used for states cache
std::uint64_t getUniqueId()
{
    std::scoped_lock lock(idMutex);

    static std::uint64_t id = 1; // start at 1, zero is "no texture"

    return id++;
}
} // namespace TextureImpl
} // namespace

namespace gfx
{
namespace resources
{

////////////////////////////////////////////////////////////
Texture::Texture()
{
    LOG_MEMORY << LOG_CONSTRUCTOR;
}


////////////////////////////////////////////////////////////
Texture::Texture(const Texture& copy)
    : m_isSmooth(copy.m_isSmooth)
    , m_sRgb(copy.m_sRgb)
    , m_isRepeated(copy.m_isRepeated)
    , m_cacheId(TextureImpl::getUniqueId())
{
    LOG_MEMORY << LOG_COPY_CONSTRUCTOR;
    if (copy.m_textureId)
    {
        if (create(copy.getSize()))
        {
            update(copy);
        }
        else
        {
            LOG_ERROR << "Failed to copy texture, failed to create new texture";
        }
    }
}


////////////////////////////////////////////////////////////
Texture::~Texture()
{
    LOG_DEBUG << LOG_BRIGHT_RED << "[destructor] Delete texture with id: " << m_textureId << LOG_WHITE;
    if (m_textureId)
    {
        gl_DeleteTexture(m_textureId);
    }
}


////////////////////////////////////////////////////////////
/*
Texture& Texture::operator=(const Texture& copy)
{
    LOG_MEMORY << LOG_COPY_ASSIGNED;
    m_actualSize = std::move(copy.m_actualSize);
    m_cacheId = std::move(copy.m_cacheId);
    m_fboAttachment = std::move(copy.m_fboAttachment);
    m_hasMipmap = std::move(copy.m_hasMipmap);
    m_isRepeated = std::move(copy.m_isRepeated);
    m_isSmooth = std::move(copy.m_isSmooth);
    m_pixelsFlipped = std::move(copy.m_pixelsFlipped);
    m_size = std::move(copy.m_size);
    m_sRgb = std::move(copy.m_sRgb);
    m_textureId = std::move(copy.m_textureId);
    return *this;
}
*/

////////////////////////////////////////////////////////////
bool Texture::create(const geometry::Size2Dpxl& size)
{
#if TEXTURE_DEBUG_MODE
    LOG_DEBUG << "Texture::create()";
    LOG_DEBUG << "size.width: " << size.width << ", size.height: " << size.height;
#endif
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

    m_size = size;

    if (not m_textureId)
    {
        GLuint texture;
        glGenTextures(1, &texture);
        m_textureId = texture;
    }
    LOG_DEBUG << "\x1B[32mCreate texture with id: " << m_textureId << "\033[0m";
    bool textureEdgeClamp = false;

    glBindTexture(GL_TEXTURE_2D, m_textureId); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(size.width), static_cast<GLsizei>(size.height), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_isRepeated ? GL_REPEAT : (textureEdgeClamp ? GL_CLAMP_TO_EDGE : GL_CLAMP));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_isRepeated ? GL_REPEAT : (textureEdgeClamp ? GL_CLAMP_TO_EDGE : GL_CLAMP));
    glGenerateMipmap(GL_TEXTURE_2D);
    m_cacheId = TextureImpl::getUniqueId();
    m_hasMipmap = false;

    return true;
}


////////////////////////////////////////////////////////////
bool Texture::createDefault(const geometry::Size2Dpxl& size)
{
    // create(size);
    gfx::resources::Image image;
    image.create({size.width, size.height}, gfx::Color(255, 255, 255, 255));
    loadFromImage(image);
    return true;
}


////////////////////////////////////////////////////////////
bool Texture::loadFromFile(const std::string& filename, const geometry::figures::Rectangle& area)
{
    gfx::resources::Image image;
    return image.loadFromFile(filename) && loadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromFileFast(const std::string& filename) //TODO
{
    /*
    if (not m_textureId)
    {
        GLuint texture;
        glGenTextures(1, &texture);
        m_textureId = texture;
    }
    LOG_DEBUG << "m_textureId: " << m_textureId;
    glBindTexture(GL_TEXTURE_2D, m_textureId); 

    m_isSmooth = false;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST);

    int width, height, nrChannels;

    auto imageLoader = common::utils::input_stream::ImageLoader::getInstance();
    std::unique_ptr<unsigned char*> ptr;
    geometry::Size2d<int> size({width, height});
    imageLoader.loadImageFromFile(filename, *ptr.get(), size);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.width, size.height, 0, GL_RGB, GL_UNSIGNED_BYTE, *ptr.get());
    glGenerateMipmap(GL_TEXTURE_2D);
    m_size.width = size.width;
    m_size.height = size.height;
    imageLoader.freeMemory(*ptr.get());
    */
    return false;
}


////////////////////////////////////////////////////////////
bool Texture::loadFromImage(const gfx::resources::Image& image, const geometry::figures::Rectangle& area)
{
    // Retrieve the image size
    int width  = static_cast<int>(image.getSize().width);
    int height = static_cast<int>(image.getSize().height);

    // Load the entire image if the source area is either empty or contains the whole image
    if (area.getSize().width == 0 || (area.getSize().height == 0) ||
    ( (area.getSize().width >= width) && (area.getSize().height >= height)))
    {
#if TEXTURE_DEBUG_MODE
        LOG_DEBUG << "Texture::loadFromImage() 1";
#endif
        if (create(image.getSize()))
        {
            update(image);

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
#if TEXTURE_DEBUG_MODE
        LOG_DEBUG << "Texture::loadFromImage() 2";
#endif
        // Load a sub-area of the image

        // Adjust the rectangle to the size of the image
        geometry::figures::Rectangle rectangle({static_cast<float>(image.getSize().width), static_cast<float>(image.getSize().height)});
        // Create the texture and upload the pixels

        if (create(geometry::Size2D({static_cast<std::uint16_t>(rectangle.getSize().width), static_cast<std::uint16_t>(rectangle.getSize().height)})))
        {
            GLint textureBinding;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureBinding);

            // Copy the pixels to the texture, row by row
            const std::uint8_t* pixels = image.getPixelsPtr() + 4 * width * height;
            glBindTexture(GL_TEXTURE_2D, m_textureId);
            for (int i = 0; i < rectangle.getSize().height; ++i)
            {
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, rectangle.getSize().width, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                pixels += 4 * width;
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST);
            m_hasMipmap = false;

            // Force an OpenGL flush, so that the texture will appear updated
            // in all contexts immediately (solves problems in multi-threaded apps)
            glFlush();

            return true;
        }
        else
        {
            return false;
        }
    }
}




////////////////////////////////////////////////////////////
unsigned int Texture::getTextureId() const
{
    return m_textureId;
}


////////////////////////////////////////////////////////////
bool Texture::isSmooth() const
{
    return m_isSmooth;
}


////////////////////////////////////////////////////////////
void Texture::setSmooth(bool smooth)
{
    if (smooth != m_isSmooth)
    {
        m_isSmooth = smooth;

        if (m_textureId)
        {
            GLint textureBinding;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureBinding);
            glBindTexture(GL_TEXTURE_2D, m_textureId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST);

            if (m_hasMipmap)
            {
                glTexParameteri(GL_TEXTURE_2D,
                                        GL_TEXTURE_MIN_FILTER,
                                        m_isSmooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST);
            }
        }
    }
}

////////////////////////////////////////////////////////////
void Texture::swap(Texture& right)
{
    std::swap(m_size, right.m_size);
    std::swap(m_actualSize, right.m_actualSize);
    std::swap(m_textureId, right.m_textureId);
    std::swap(m_isSmooth, right.m_isSmooth);
    std::swap(m_sRgb, right.m_sRgb);
    std::swap(m_isRepeated, right.m_isRepeated);
    std::swap(m_pixelsFlipped, right.m_pixelsFlipped);
    std::swap(m_fboAttachment, right.m_fboAttachment);
    std::swap(m_hasMipmap, right.m_hasMipmap);

    m_cacheId       = TextureImpl::getUniqueId();
    right.m_cacheId = TextureImpl::getUniqueId();
}

////////////////////////////////////////////////////////////
unsigned int Texture::getMaximumSize()
{
    std::scoped_lock lock(TextureImpl::maximumSizeMutex);

    static bool  checked = false;
    static GLint size    = 0;

    if (not checked)
    {
        checked = true;

        // TransientContextLock transientLock; TODO

        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
    }

    return static_cast<unsigned int>(size);
}


////////////////////////////////////////////////////////////
void Texture::update(const gfx::resources::Image& image)
{
    // Update the whole texture
    update(image.getPixelsPtr(), {image.getSize().width, image.getSize().height}, {0, 0});
}


////////////////////////////////////////////////////////////
void Texture::update(const std::uint8_t* pixels)
{
    // Update the whole texture
    update(pixels, {m_size.width, m_size.height}, {0, 0});
}


////////////////////////////////////////////////////////////
void Texture::update(const Texture& texture)
{
    // Update the whole texture
    update(texture, {0, 0});
}


////////////////////////////////////////////////////////////
void Texture::update(const Texture& texture, const geometry::Vector2Du& dest)
{
    assert(dest.x + texture.m_size.width <= m_size.width);
    assert(dest.y + texture.m_size.height <= m_size.height);


    if (not m_textureId or not texture.m_textureId)
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
                                             texture.m_textureId,
                                             0);

        // Link the destination texture to the destination frame buffer
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFrameBuffer);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0);

        // A final check, just to be sure...
        GLenum sourceStatus;
        sourceStatus = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);

        GLenum destStatus;
        destStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

        if ((sourceStatus == GL_FRAMEBUFFER_COMPLETE) and (destStatus == GL_FRAMEBUFFER_COMPLETE))
        {
            // Blit the texture contents from the source to the destination texture
            glBlitFramebuffer(0,
                    texture.m_pixelsFlipped ? static_cast<GLint>(texture.m_size.height) : 0,
                    static_cast<GLint>(texture.m_size.width),
                    texture.m_pixelsFlipped ? 0 : static_cast<GLint>(texture.m_size.height), // Source rectangle, flip y if source is flipped
                    static_cast<GLint>(dest.x),
                    static_cast<GLint>(dest.y),
                    static_cast<GLint>(dest.x + texture.m_size.width),
                    static_cast<GLint>(dest.y + texture.m_size.height), // Destination rectangle
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
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST);
        m_hasMipmap     = false;
        m_pixelsFlipped = false;
        m_cacheId       = TextureImpl::getUniqueId();

        // Force an OpenGL flush, so that the texture data will appear updated
        // in all contexts immediately (solves problems in multi-threaded apps)
        glFlush();
        return;
    }

    // TODO
    LOG_ERROR << "no implemented";
}


////////////////////////////////////////////////////////////
void Texture::bind(const Texture* texture, CoordinateType coordinateType)
{
    /*
    //TransientContextLock lock;

    if (texture && texture->m_textureId)
    {
        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, texture->m_textureId);

        // Check if we need to define a special texture matrix
        if ((coordinateType == Pixels) || texture->m_pixelsFlipped)
        {
            // clang-format off
            GLfloat matrix[16] = {1.f, 0.f, 0.f, 0.f,
                                  0.f, 1.f, 0.f, 0.f,
                                  0.f, 0.f, 1.f, 0.f,
                                  0.f, 0.f, 0.f, 1.f};
            // clang-format on

            // If non-normalized coordinates (= pixels) are requested, we need to
            // setup scale factors that convert the range [0 .. size] to [0 .. 1]
            if (coordinateType == Pixels)
            {
                matrix[0] = 1.f / static_cast<float>(texture->m_actualSize.width);
                matrix[5] = 1.f / static_cast<float>(texture->m_actualSize.height);
            }

            // If pixels are flipped we must invert the Y axis
            if (texture->m_pixelsFlipped)
            {
                matrix[5]  = -matrix[5];
                matrix[13] = static_cast<float>(texture->m_size.height) / static_cast<float>(texture->m_actualSize.height);
            }

            // Load the matrix
            glMatrixMode(GL_TEXTURE);
            glLoadMatrixf(matrix);

            // Go back to model-view mode (sf::RenderTarget relies on it)
            glMatrixMode(GL_MODELVIEW);
        }
    }
    else
    {
        // Bind no texture
        glBindTexture(GL_TEXTURE_2D, 0);

        // Reset the texture matrix
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();

        // Go back to model-view mode (sf::RenderTarget relies on it)
        glMatrixMode(GL_MODELVIEW);
    }
    */
}


////////////////////////////////////////////////////////////
void Texture::update(const std::uint8_t* pixels, const geometry::Size2Dui& size, const geometry::Vector2Dui& dest)
{
    assert(dest.x + size.width <= m_size.width);
    assert(dest.y + size.height <= m_size.height);

    if (pixels and m_textureId)
    {
#if TEXTURE_DEBUG_MODE
        LOG_DEBUG << "Texture::update() , m_textureId: " << m_textureId << ". size: " << size << ", dest: " << dest;;
#endif
        // Make sure that the current texture binding will be preserved

        GLint textureBinding;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureBinding);

        // Copy pixels from the given array to the texture
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glTexSubImage2D(GL_TEXTURE_2D,
                                0,
                                static_cast<GLint>(dest.x),
                                static_cast<GLint>(dest.y),
                                static_cast<GLsizei>(size.width),
                                static_cast<GLsizei>(size.height),
                                GL_RGBA,
                                GL_UNSIGNED_BYTE,
                                pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_isSmooth ? GL_LINEAR : GL_NEAREST);
        m_hasMipmap     = false;
        m_pixelsFlipped = false;
        m_cacheId       = TextureImpl::getUniqueId();

        // Force an OpenGL flush, so that the texture data will appear updated
        // in all contexts immediately (solves problems in multi-threaded apps)
        glFlush();
    }
}


////////////////////////////////////////////////////////////
geometry::Size2D Texture::getSize() const
{
    return m_size;
}
} // namespace resources
} // namespace gfx