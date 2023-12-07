#include "gfx/draw/Button.hpp"

#include "gfx/draw/Text.hpp"
#include "gfx/render/Render.hpp"
#include "gfx/graphic/Texture.hpp"
#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
////////////////////////////////////////////////////////////
Button::Button(const std::string& name, const std::shared_ptr<he::gfx::render::ITexture>& texture) : Sprite(name, texture)
{
}


////////////////////////////////////////////////////////////
Button::Button(const std::string& name, const geometry::Size2Dpxl& size) : Sprite(name, std::make_shared<he::gfx::render::Texture>(size))
{
}


////////////////////////////////////////////////////////////
Button::Button(const Button& copy)
    : m_text{copy.m_text ? std::make_unique<gfx::draw::Text>(*copy.m_text) : nullptr}
    , Sprite(copy)
{
}


//////////////////////////////////////////////////////////////////////
void Button::draw(gfx::render::Render& render, const gfx::render::RenderSettings& renderSettings)
{
    Sprite::draw(render, renderSettings); 
    if (m_text != nullptr)
    {
        render.draw(*m_text, renderSettings);
    }
}


//////////////////////////////////////////////////////////////////////
void Button::setCallback(const ButtonCallback& callback, const window::Event event)
{
    removeCallback(event);
    m_callbackMap.insert(std::make_pair(event, callback));
}


//////////////////////////////////////////////////////////////////////
void Button::removeCallback(const window::Event event)
{
    auto it = m_callbackMap.find(event);

    if (it != m_callbackMap.end())
    {
        m_callbackMap.erase(it->first);
        LOG_DEBUG << "Remove callback: " << he::window::toString(event.type);
    }
}


//////////////////////////////////////////////////////////////////////
void Button::setPosition(const geometry::Point2Df& point)
{
    Shape::setPosition(point);
    updateTextPosition();
}


////////////////////////////////////////////////////////////
void Button::setOriginInCenter()
{
    Shape::setOriginInCenter();
    updateTextPosition();
}


//////////////////////////////////////////////////////////////////////
bool Button::onMauseButtonPressed(const he::window::Event::MouseButtonAction& event)
{
    auto it = m_callbackMap.find(he::window::Event(he::window::Event::mouseButtonPressed, event));

    if (it != m_callbackMap.end() and isPointInside(event.x, event.y)) 
    {
        LOG_DEBUG << "Process event: MouseButtonAction, layer: " << m_context.layerName << ", button: " << m_context.name;
        it->second();
        return true;
    } 
    return false;
}


////////////////////////////////////////////////////////////
bool Button::onMouseCursorMoved(const he::window::Event::MouseMoveEvent& event)
{
    auto it = m_callbackMap.find(he::window::Event(he::window::Event::mouseCursorMoved));

    if (it != m_callbackMap.end() and isPointInside(event.x, event.y)) 
    {
        LOG_DEBUG << "Process event: mouseCursorMoved, layer: " << m_context.layerName << ", button: " << m_context.name;
        it->second();
        return true;
    } 

    return false;
}

//////////////////////////////////////////////////////////////////////
void Button::setText(const std::string& text)
{
    if (m_text == nullptr)
    {
        setDefaultTextSettings();
    }
    m_text->setString(text);
    updateTextPosition();
}


//////////////////////////////////////////////////////////////////////
void Button::setText(const draw::Text& text)
{
    m_text = std::make_unique<draw::Text>(text);
    // todo change name function to connect text with button
    updateTextPosition();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
void Button::updateTextPosition()
{
    if (m_text)
    {
        m_text->setOriginInCenter();
        auto textPosition = m_text->getPosition();
        auto textPosCorrectionX = textPosition.x - m_position.x;
        auto textPosCorrectionY = textPosition.y - m_position.y;
        m_text->setPosition({textPosition.x - textPosCorrectionX + m_figure->getSize().width/2.f - getOrigin().x, 
            textPosition.y - textPosCorrectionY + m_figure->getSize().height/2.f - getOrigin().y});
    }
}


//////////////////////////////////////////////////////////////////////
void Button::setDefaultTextSettings()
{
    const std::string buttonTextName = m_context.name + "_text";
    m_text = std::make_unique<gfx::draw::Text>(buttonTextName);
    m_text->setColor({gfx::Color::White});
    m_text->setFont("\\data\\gfx\\fonts\\calibri.ttf");
    m_text->setCharacterSize(30);
}


//////////////////////////////////////////////////////////////////////
bool Button::isPointInside(const int x, const int y)
{
    if (Shape::isPointInside({static_cast<float>(x), static_cast<float>(y)}))
    {
        return true;
    }
    return false;
}


} // namespace draw
} // namespace gfx
} // namespace he