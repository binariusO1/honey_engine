#pragma once

#include "gfx/draw/Sprite.hpp"
#include "window/events/IEventInputListener.hpp"

namespace he
{
namespace gfx
{
namespace render
{
class Button : public draw::Sprite , public he::window::IEventInputListener
{
public:
    Button(const std::string&);
    ~Button() = default;

public:
    void process_event(const he::window::Event&) override;
};
} // namespace render
} // namespace gfx
} // namespace he