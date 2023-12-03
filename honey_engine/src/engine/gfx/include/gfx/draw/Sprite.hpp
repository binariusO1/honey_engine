#pragma once

#include "gfx/draw/ShapeRectangle.hpp"

namespace he
{
namespace gfx
{
namespace render
{
class ITexture;
} // namespace render
namespace draw
{
class Sprite : public draw::ShapeRectangle
{
public:
    Sprite() = delete;
    explicit Sprite(const std::string&, const std::shared_ptr<he::gfx::render::ITexture>&);
    Sprite(const Sprite&);
    ~Sprite() override;

public:
    unsigned int getTextureId() const;
    void setPosition(const he::gfx::geometry::Point2Df& position);

public:
    void setColor(const he::gfx::Color& color) override;
    void unsetColor();

protected:
    void draw(he::gfx::render::Render&, const he::gfx::render::RenderSettings&) override;
    void update();
    
protected:
    void updateTextureCoords();

    std::shared_ptr<he::gfx::render::ITexture> m_texture{nullptr};
    bool m_isFilledByColor{false};
    bool m_textureCoordsNeedUpdate{false};
};
} // namespace draw
} // namespace gfx
} // namespace he