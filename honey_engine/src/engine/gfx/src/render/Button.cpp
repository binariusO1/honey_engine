#include "gfx/render/Button.hpp"

#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace render
{
////////////////////////////////////////////////////////////
Button::Button(const std::string& name) : Sprite(name, nullptr)
{
    //
}


////////////////////////////////////////////////////////////
void Button::process_event(const he::window::Event& event)
{
    //
}
} // namespace render
} // namespace gfx
} // namespace he