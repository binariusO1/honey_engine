#pragma once

#include "gfx/draw/Sprite.hpp"
#include "gfx/draw/Text.hpp"
#include "window/events/IEventInputListener.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
class Button : public draw::Sprite , public he::window::IEventInputListener
{
public:
    Button(const std::string&, const std::shared_ptr<he::gfx::render::ITexture>& texture);
    Button(const Button&);
    ~Button() = default;

public:
    void draw(gfx::render::Render& render, const gfx::render::RenderSettings& renderSettings);

public:
    void process_event(const he::window::Event&) override;

public:
    void setText(const std::string&);

private:
    std::unique_ptr<Text> m_text{nullptr};
};
} // namespace draw
} // namespace gfx
} // namespace he