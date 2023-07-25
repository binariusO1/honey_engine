#pragma once

#include <string>
#include "geometry/figures/Rectangle.hpp"
#include "geometry/Size2d.hpp"
#include "geometry/Vector2d.hpp"
#include "gl_wrapper/resources/GlResource.hpp"


namespace gfx
{
namespace resources
{
class Image;

class Texture : public gl_wrapper::GlResource
{
public:
    enum CoordinateType
    {
        Normalized,
        Pixels
    };
public:
    Texture();
    Texture(const Texture&);
    ~Texture();

public:
    //Texture& operator=(const Texture&);

public:
    bool create(const geometry::Size2Dpxl& size);
    bool createDefault(const geometry::Size2Dpxl& size);

    bool loadFromFile(const std::string& filename, const geometry::figures::Rectangle& rectangle = geometry::figures::Rectangle());
    bool loadFromFileFast(const std::string& filename);
    // bool loadFromMemory(const void* data, std::size_t size, const IntRect& area = IntRect());
    // bool loadFromStream(InputStream& stream, const IntRect& area = IntRect());
    bool loadFromImage(const gfx::resources::Image& image, const geometry::figures::Rectangle& rectangle = geometry::figures::Rectangle());

    void bind(const Texture* texture, CoordinateType coordinateType);

    geometry::Size2D getSize() const;
    void update(const gfx::resources::Image& image);
    void update(const std::uint8_t* pixels);
    void update(const Texture& texture);
    void update(const Texture& texture, const geometry::Vector2Du& dest);
    void update(const std::uint8_t* pixels, const geometry::Size2Dui& size, const geometry::Vector2Dui& dest);
    void swap(Texture&);

public:
    unsigned int getTextureId() const;
    static unsigned int getMaximumSize();

public:
    void setSmooth(bool smooth);
    bool isSmooth() const;

private:
    geometry::Size2D    m_size{};
    geometry::Size2D    m_actualSize{}; // with padding
    unsigned int        m_textureId{0}; //id handler
    bool                m_sRgb{false};          //!< Should the texture source be converted from sRGB?
    bool                m_isRepeated{false};    //!< Is the texture in repeat mode?
    bool                m_isSmooth{false};
    bool                m_hasMipmap{false};
    mutable bool        m_pixelsFlipped{false};
    bool                m_fboAttachment{false};
    std::uint64_t       m_cacheId; 
};
} // namespace resources
} // namespace gfx