#pragma once

#include <memory>
#include "gfx/geometry/Vector2d.hpp"
#include "gfx/graphic/ITexture.hpp"

namespace he
{
namespace libs::gl
{
class IGlWrapper;
} // namespace libs::gl
namespace gfx
{
namespace render
{
class Image;
class Texture : public ITexture
{
public:
    Texture();
    Texture(const geometry::Size2Dpxl& size);
    Texture(const std::string& filepath);
    Texture(const he::gfx::render::Image);
    ~Texture() override;

public:
    bool create(const geometry::Size2Dpxl& size);
    bool create(const geometry::Size2Dpxl&, const std::uint8_t*) override;
    bool createEmpty(const he::gfx::geometry::Size2Dpxl& size) override;
    void setSmooth(bool) override;
    const he::gfx::geometry::Size2Dpxl getSize() const override;
    const unsigned int getTextureId() const override;
    void update(const Texture& texture);
    void update(const Texture& texture, const geometry::Vector2Du& dest);
    void update(const std::uint8_t* pixels, const geometry::Size2Dui& size, const geometry::Vector2Dui& destination);
    void swap(Texture& right);

public:
    const unsigned int getMaxTextureSize();

private:
    bool prepareTexture(const geometry::Size2Dpxl& size);
    void createTexture(const geometry::Size2Dpxl& size, const std::uint8_t* pixels);

protected:
    ITexture::Context m_context;
    std::shared_ptr<he::libs::gl::IGlWrapper> m_glWrapper;
    bool m_isSmooth{false};
    bool m_hasMipmap{false};
    bool m_pixelsFlipped{false};
    bool m_isRepeated{false};
};
} // namespace render
} // namespace gfx
} // namespace he