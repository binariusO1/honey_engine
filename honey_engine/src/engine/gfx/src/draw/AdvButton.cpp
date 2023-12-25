#include "gfx/draw/AdvButton.hpp"

#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
////////////////////////////////////////////////////////////
AdvButton::AdvButton(const std::string& name, const std::shared_ptr<he::gfx::render::ITexture>& texture) : Button(name, texture)
{
}


////////////////////////////////////////////////////////////
AdvButton::AdvButton(const std::string& name, const geometry::Size2Dpxl& size) : Button(name, size)
{
}


////////////////////////////////////////////////////////////
AdvButton::AdvButton(const AdvButton& copy)
    : Button(copy)
{
}


////////////////////////////////////////////////////////////
void AdvButton::setCallback(const ButtonCallback& callback, const ButtonTransition& transition)
{
    removeCallback(transition);
    m_transCallbackMap.insert(std::make_pair(transition, callback));
}


////////////////////////////////////////////////////////////
void AdvButton::removeCallback(const ButtonTransition& transition)
{
    auto it = m_transCallbackMap.find(transition);

    if (it != m_transCallbackMap.end())
    {
        m_transCallbackMap.erase(it->first);
        LOG_DEBUG << "Remove callback: " << he::gfx::toString(transition.getOldState()) << "->" << he::gfx::toString(transition.getCurrentState());
    }
}


//////////////////////////////////////////////////////////////////////
bool AdvButton::onMouseButtonPressed(const he::window::Event::MouseButtonAction& mouseButtonAction)
{
    window::Event event{he::window::Event(he::window::Event::mouseButtonPressed, mouseButtonAction)};
    return isStateChanged(event);
}


//////////////////////////////////////////////////////////////////////
bool AdvButton::onMouseButtonReleased(const he::window::Event::MouseButtonAction& mouseButtonAction)
{
    window::Event event{he::window::Event(he::window::Event::mouseButtonReleased, mouseButtonAction)};
    return isStateChanged(event);
}


////////////////////////////////////////////////////////////
bool AdvButton::onMouseCursorMoved(const he::window::Event::MouseMoveEvent& mouseMoveEvent)
{
    window::Event event(window::Event::mouseCursorMoved, mouseMoveEvent);
    return isStateChanged(event);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
bool AdvButton::isStateChanged(const window::Event& event)
{
    auto shouldRunCallback{false};

    switch (this->m_buttonTransition.getCurrentState())
    {
        case ButtonState::Idle:
            shouldRunCallback = handleStateIdle(event);
            break;
        case ButtonState::Touched:  
            shouldRunCallback = handleStateTouched(event);
            break;
        case ButtonState::Clicked:  
            shouldRunCallback = handleStateClicked(event);
            break;
        default:
            break;
    }

    if (shouldRunCallback)
    {
        return this->runCallback();
    }
    return false;
}


////////////////////////////////////////////////////////////
bool AdvButton::handleStateIdle(const window::Event& event)
{
    switch (event.type)
    {
        case window::Event::mouseCursorMoved:
        {
            if (isPointInside({static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y)}))
            {
                this->setState(ButtonState::Touched);
                return true;
            }
        }
        return false;

        default:
            return false;
    }
}


////////////////////////////////////////////////////////////
bool AdvButton::handleStateTouched(const window::Event& event)
{
    switch (event.type)
    {
        case window::Event::mouseCursorMoved:
        {
            if (not isPointInside({static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y)}))
            {
                this->setState(ButtonState::Idle);
                return true;
            }
        }
        return false;

        case window::Event::mouseButtonPressed:
        {
            this->setState(ButtonState::Clicked);
            return true;
        }
        return false;

        default:
            return false;
    }
}


////////////////////////////////////////////////////////////
bool AdvButton::handleStateClicked(const window::Event& event)
{
    switch (event.type)
    {
        case window::Event::mouseButtonReleased:
        {
            const bool isPointInside = Button::isPointInside({static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)});
    
            if (isPointInside)
            {
                this->setState(ButtonState::Touched);
                return true;
            }
            else if (not isPointInside)
            {
                this->setState(ButtonState::Idle);
            return true;
            }
        }
        return false;

        default:
            return false;
    }
}


////////////////////////////////////////////////////////////
void AdvButton::setState(const ButtonState state)
{
    m_buttonTransition.update(state);
    LOG_DEBUG << "Button: " << m_context.layerName << ", " << m_context.name 
        << ", setState: " << he::gfx::toString(m_buttonTransition.getOldState()) << "->" << he::gfx::toString(m_buttonTransition.getCurrentState());
}


////////////////////////////////////////////////////////////
bool AdvButton::runCallback()
{
    auto it = m_transCallbackMap.find(m_buttonTransition);

    if (it != m_transCallbackMap.end())
    {
         it->second();
         return true;
    }
    return false;
}


////////////////////////////////////////////////////////////
void AdvButton::setCallback(const ButtonCheckCallback& callback, const window::Event event)
{
    Button::setCallback(callback, event);
}



////////////////////////////////////////////////////////////
    void AdvButton::removeCallback(const window::Event event)
{
    Button::removeCallback(event);
}
} // namespace draw
} // namespace gfx
} // namespace he