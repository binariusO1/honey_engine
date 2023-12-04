#pragma once

#include <map>
#include <functional>
#include "gfx/draw/Sprite.hpp"
#include "gfx/draw/Text.hpp"
#include "gfx/draw/IButton.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
using ButtonCallback = std::function<void()>;

class Button : public draw::Sprite , public draw::IButton
{
public:
    Button() = delete;
    Button(const std::string&, const std::shared_ptr<he::gfx::render::ITexture>& texture);
    Button(const std::string&, const geometry::Size2Dpxl& size);
    Button(const Button&);
    ~Button() = default;

public:
    void setPosition(const geometry::Point2Df&);
    void setOriginInCenter() override;
    void draw(gfx::render::Render& render, const gfx::render::RenderSettings& renderSettings) override;

public:
    void setCallback(const ButtonCallback& callback, const window::Mouse::Button mouseButton);
    void removeCallback(const window::Mouse::Button mouseButton);
    bool onMauseButtonPressed(const he::window::Event::MouseButtonAction&) override;
    
public:
    void setText(const std::string&);
    void setText(const draw::Text&);

private:
    void updateTextPosition();
    void setDefaultTextSettings();
    bool isPointInside(const int x, const int y);

private:
    std::unique_ptr<Text> m_text{nullptr};
    std::map<window::Mouse::Button, ButtonCallback> m_callbackMap;
};
} // namespace draw
} // namespace gfx
} // namespace he