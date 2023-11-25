#include "gfx/draw/Button.hpp"

#include "gfx/draw/Text.hpp"
#include "gfx/render/Render.hpp"
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
void Button::setPosition(const geometry::Point2Df& point)
{
    Shape::setPosition(point);
    setTextPosition();
}


//////////////////////////////////////////////////////////////////////
bool Button::onMauseButtonPressed(const he::window::Event::MouseButtonAction& event)
{
    if (isPointInside(event.x, event.y))
    {
        LOG_DEBUG << "Process event: MouseButtonAction, button name: " << m_context.name;
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
    setTextPosition();
}


//////////////////////////////////////////////////////////////////////
void Button::setText(const draw::Text& text)
{
    m_text = std::make_unique<draw::Text>(text);
    // todo change name function to connect text with button
    setTextPosition();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
void Button::setTextPosition()
{
    if (m_text)
    {
        m_text->setOriginInCenter();
        auto textPosition = m_text->getPosition();
        auto textPosCorrectionX = textPosition.x - m_position.x;
        auto textPosCorrectionY = textPosition.y - m_position.y;
        m_text->setPosition({textPosition.x - textPosCorrectionX + m_figure.getSize().width/2.f - getOrigin().x, textPosition.y - textPosCorrectionY + m_figure.getSize().height/2.f - getOrigin().y});
    }
}


//////////////////////////////////////////////////////////////////////
void Button::setDefaultTextSettings()
{
    const std::string buttonTextName = m_context.name + "_text";
    m_text = std::make_unique<gfx::draw::Text>(buttonTextName);
    m_text->setColor({gfx::Color::White});
    m_text->setFont("\\data\\gfx\\fonts\\calibri.ttf");
    m_text->setCharacterSize(40);
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