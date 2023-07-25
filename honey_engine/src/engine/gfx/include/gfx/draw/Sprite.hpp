#pragma once

#include "gfx/draw/Shape.hpp"

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
class Sprite : public Shape
{
public:
    Sprite() = delete;
    explicit Sprite(const std::string&, const std::shared_ptr<he::gfx::render::ITexture>&);
    ~Sprite() override;

public:
    unsigned int getTextureId() const;
    void setPosition(const he::gfx::geometry::Point2Df& position);

protected:
    void draw(he::gfx::render::Render&, he::gfx::render::RenderSettings&) const override;

private:
    void updateTextureCoords();
    std::shared_ptr<he::gfx::render::ITexture>  m_texture{nullptr};
};
} // namespace draw
} // namespace gfx
} // namespace he