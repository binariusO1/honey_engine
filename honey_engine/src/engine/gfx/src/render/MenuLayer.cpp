#include "gfx/render/MenuLayer.hpp"

#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace render
{
////////////////////////////////////////////////////////////
MenuLayer::MenuLayer(const std::string& name, const PropagationSettings& propagationSettings) 
    : UniquePropagationLayer(name, propagationSettings)
{
}


////////////////////////////////////////////////////////////
void MenuLayer::render(gfx::render::IRender& render)
{
    UniquePropagationLayer::render(render);
}


////////////////////////////////////////////////////////////
bool MenuLayer::setPosition(const geometry::Point2Df& position)
{
    checkAndCorrectPropagationParameters();
    switch (m_originPosition)
    {
        case he::gfx::OriginPosition::any:
            LOG_WARNING << "OriginPosition::any has no efect";
            return false;
        case he::gfx::OriginPosition::leftDown:
            return false;
        case he::gfx::OriginPosition::center:
            setPositionToCenter(position);
            return true;
        default:
            break;
    }

    return false;
}


////////////////////////////////////////////////////////////
void MenuLayer::setOrigin(gfx::OriginPosition originPosition)
{
    m_originPosition = originPosition;
}


////////////////////////////////////////////////////////////
void MenuLayer::setButton(const std::shared_ptr<gfx::draw::Button>& button)
{
    if (isButtonSet)
    {
        LOG_DEBUG << "Overriding existed button";
    }

    isButtonSet = true;
    m_buttons.clear();
    addButton(button);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
void MenuLayer::addButton(const std::shared_ptr<gfx::draw::Button>& button)
{
    UniquePropagationLayer::addButton(button);
}


////////////////////////////////////////////////////////////
void MenuLayer::setPositionToCenter(const geometry::Point2Df& position)
{
    if (m_buttons.size() > 0)
    {
        auto numX = m_propagationSettings.numberOfElementsX;
        auto numY = m_propagationSettings.numberOfElementsY;
        auto buttonWidth = m_buttons[0]->getSize().width;
        auto buttonHeight = m_buttons[0]->getSize().height;
        float newPosX{position.x};
        float newPosY{position.y};

        m_buttons[0]->setOriginInCenter();
        auto propWidth = (buttonWidth * numX) + ((numX - 1) * std::abs(m_propagationSettings.distanceX));
        auto propHeight = (buttonHeight * numY) + ((numY - 1) * std::abs(m_propagationSettings.distanceY));

        if (m_propagationSettings.distanceX > 0)
        {
            newPosX = position.x - propWidth/2.f;
        }
        else if (m_propagationSettings.distanceX < 0)
        {
            newPosX = position.x + propWidth/2.f - buttonWidth / 2.f;
        }

        if (m_propagationSettings.distanceY > 0)
        {
            newPosY = position.y - propHeight/2.f;
        }
        else if (m_propagationSettings.distanceY < 0)
        {
            newPosY = position.y + propHeight/2.f - buttonHeight / 2.f;
        }

        m_buttons[0]->setPosition({newPosX, newPosY});
        updateButtons(0);
    }
}

} // namespace render
} // namespace gfx
} // namespace he