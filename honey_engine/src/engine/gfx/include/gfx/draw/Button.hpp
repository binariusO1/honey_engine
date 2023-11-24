#pragma once

#include "gfx/draw/Sprite.hpp"
#include "gfx/draw/Text.hpp"
#include "gfx/draw/IButton.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
class Button : public draw::Sprite , public draw::IButton
{
public:
    Button(const std::string&, const std::shared_ptr<he::gfx::render::ITexture>& texture);
    Button(const Button&);
    ~Button() = default;

public:
    void setPosition(const geometry::Point2Df&);
    void draw(gfx::render::Render& render, const gfx::render::RenderSettings& renderSettings) override;

public:
    bool onMauseButtonPressed(const he::window::Event::MouseButtonAction&) override;
    
public:
    void setText(const std::string&);

private:
    void setTextPosition(const geometry::Point2Df& point);
    bool isPointInside(const int x, const int y);

private:
    std::unique_ptr<Text> m_text{nullptr};
};
} // namespace draw
} // namespace gfx
} // namespace he