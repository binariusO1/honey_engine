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
    //
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
    setTextPosition(point);
    Shape::setPosition(point);
}


////////////////////////////////////////////////////////////
void Button::process_event(const he::window::Event& event)
{
    //
}


//////////////////////////////////////////////////////////////////////
void Button::setText(const std::string& text)
{
    if (m_text == nullptr)
    {
        const std::string buttonTextName = m_context.name + "_text";
        m_text = std::make_unique<gfx::draw::Text>(buttonTextName);
        m_text->setColor({gfx::Color::White});
        m_text->setPosition({m_position.x + m_origin.x, m_position.y + m_origin.y});
        m_text->setFont("\\data\\gfx\\fonts\\calibri.ttf");
        m_text->setCharacterSize(40);
        m_text->setOriginInCenter();
    }
    m_text->setString(text);
}


//////////////////////////////////////////////////////////////////////
void Button::setTextPosition(const geometry::Point2Df& point)
{
    if (m_text)
    {
        auto textPosition = m_text->getPosition();
        auto textPosCorrectionX = textPosition.x - getPosition().x;
        auto textPosCorrectionY = textPosition.y - getPosition().y;
        m_text->setPosition({point.x + textPosCorrectionX, point.y + textPosCorrectionY});
    }
}
} // namespace draw
} // namespace gfx
} // namespace he