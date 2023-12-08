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
using ButtonCallback = std::function<void(bool isPointInside)>;
using ButtonEventCallbackMap = std::unordered_map<window::Event, ButtonCallback, window::EventHasher>;

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
    void setCallback(const ButtonCallback& callback, const window::Event event);
    void removeCallback(const window::Event event);
    bool onMouseButtonPressed(const he::window::Event::MouseButtonAction&) override;
    bool onMouseButtonReleased(const he::window::Event::MouseButtonAction&) override;
    bool onMouseCursorMoved(const he::window::Event::MouseMoveEvent&) override;

public:
    void setText(const std::string&);
    void setText(const draw::Text&);
    bool isPointInside(const int x, const int y);//todo: Point2i

private:
    void updateTextPosition();
    void setDefaultTextSettings();
    bool checkEvent(const window::Event& event, const gfx::geometry::Point2Di point);

private:
    std::unique_ptr<Text> m_text{nullptr};
    ButtonEventCallbackMap m_callbackMap;
};
} // namespace draw
} // namespace gfx
} // namespace he