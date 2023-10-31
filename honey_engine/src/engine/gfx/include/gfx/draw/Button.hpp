#pragma once

#include "gfx/draw/Sprite.hpp"
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
    ~Button() = default;

public:
    void process_event(const he::window::Event&) override;

protected:
    void draw(const he::gfx::render::Render&, const he::gfx::render::RenderSettings&) const override;
};
} // namespace draw
} // namespace gfx
} // namespace he