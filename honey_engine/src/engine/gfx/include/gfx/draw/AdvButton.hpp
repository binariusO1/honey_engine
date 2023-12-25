#pragma once

#include "gfx/draw/Button.hpp"
#include "gfx/draw/ButtonTransition.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
using ButtonCallback = std::function<void()>;
using ButtonTransitionCallbackMap = std::unordered_map<ButtonTransition, ButtonCallback, ButtonTransitionHasher>;

// todo: kolejna wersja to GfxButton
class AdvButton : public draw::Button
{
public:
    AdvButton() = delete;
    AdvButton(const std::string&, const std::shared_ptr<he::gfx::render::ITexture>& texture);
    AdvButton(const std::string&, const geometry::Size2Dpxl& size);
    AdvButton(const AdvButton&);
    ~AdvButton() = default;

public:
    void setCallback(const ButtonCallback&, const ButtonTransition&);
    void removeCallback(const ButtonTransition&);

public:
    bool onMouseButtonPressed(const he::window::Event::MouseButtonAction&) override;
    bool onMouseButtonReleased(const he::window::Event::MouseButtonAction&) override;
    bool onMouseCursorMoved(const he::window::Event::MouseMoveEvent&) override;

protected:
    void setCallback(const ButtonCheckCallback& callback, const window::Event event) override;
    void removeCallback(const window::Event event) override;

private:
    void setState(const ButtonState);
    bool isStateChanged(const window::Event& event);
    bool runCallback();
    bool handleStateIdle(const window::Event& event);
    bool handleStateTouched(const window::Event& event);
    bool handleStateClicked(const window::Event& event);

private:
    ButtonTransition m_buttonTransition;
    ButtonTransitionCallbackMap m_transCallbackMap{};
};
} // namespace draw
} // namespace gfx
} // namespace he