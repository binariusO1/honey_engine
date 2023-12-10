#include "gfx/render/UniquePropagationLayer.hpp"

#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace render
{
////////////////////////////////////////////////////////////
UniquePropagationLayer::UniquePropagationLayer(const std::string& name, const PropagationSettings& propagationSettings) 
    : Layer(name)
    , m_propagationSettings{propagationSettings}
{
}


////////////////////////////////////////////////////////////
UniquePropagationLayer::~UniquePropagationLayer()
{
}


////////////////////////////////////////////////////////////
void UniquePropagationLayer::render(gfx::render::IRender& render)
{
    Layer::render(render);
}


////////////////////////////////////////////////////////////
void UniquePropagationLayer::addButton(const std::shared_ptr<gfx::draw::Button>& button)
{
    checkAndCorrectPropagationParameters();

    auto startIndex = m_buttons.size();
    auto numberOfButtons = m_propagationSettings.numberOfElementsX * m_propagationSettings.numberOfElementsY;
    auto numX = m_propagationSettings.numberOfElementsX;
    auto numY = m_propagationSettings.numberOfElementsY;

    Layer::addButton(button);

    for (std::size_t x = 1; x <= numX; ++x)
    {
        for (std::size_t y = 1; y <= numY ; ++y)
        {
            if (x == 1 and y == 1)
            {
                continue;
            }

            std::shared_ptr<he::gfx::draw::Button> newButton = std::make_shared<he::gfx::draw::Button>(*button);
            newButton->setName(newButton->getName() + "_x" + std::to_string(x) + "_y" + std::to_string(y));
            Layer::addButton(newButton);
        }
    }
    updateButtons(startIndex);
}


////////////////////////////////////////////////////////////
void UniquePropagationLayer::update()
{
    auto numOfElementsInPropagation = m_propagationSettings.numberOfElementsX * m_propagationSettings.numberOfElementsY;

    for (std::size_t i = 0 ; i < m_buttons.size() ; i+=numOfElementsInPropagation)
    {
        updateButtons(i);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
void UniquePropagationLayer::checkAndCorrectPropagationParameters()
{
    if (m_propagationSettings.numberOfElementsX == 0)
    {
        m_propagationSettings.numberOfElementsX = 1;

        if (m_propagationSettings.numberOfElementsX == 1 and m_propagationSettings.distanceX > 0)
        {
            m_propagationSettings.distanceX = 0;
        }
    }

    if (m_propagationSettings.numberOfElementsY == 0)
    {
        m_propagationSettings.numberOfElementsY = 1;

        if (m_propagationSettings.numberOfElementsY == 1 and m_propagationSettings.distanceY > 0)
        {
            m_propagationSettings.distanceY = 0;
        }
    }
}


////////////////////////////////////////////////////////////
void UniquePropagationLayer::updateButtons(const std::size_t startIndex)
{
    if (m_buttons.size() <= 1)
    {
        return;
    }

    const auto firstButtonPosition = m_buttons[startIndex]->getPosition();
    auto numX = m_propagationSettings.numberOfElementsX;
    auto numY = m_propagationSettings.numberOfElementsY;
    float xPosition{0.f};
    float yPosition{0.f};
    std::size_t i{startIndex};

    for (std::size_t x = 1; x <= numX; ++x)
    {
        xPosition = calcXPos(m_buttons[startIndex].get(), x);

        for (std::size_t y = 1; y <= numY; ++y, ++i)
        {
            yPosition = calcYPos(m_buttons[startIndex].get(), y);

            if (x == 1 and y == 1)
            {
                continue;
            }

            if (x == 1 and y > 1)
            {
                xPosition = firstButtonPosition.x;
            }
            else if (x > 1 and y == 1)
            {
                yPosition = firstButtonPosition.y;
            }

            m_buttons[i]->setOrigin(m_buttons[startIndex]->getOrigin());
            m_buttons[i]->setPosition({xPosition, yPosition});
        }
    }
}


////////////////////////////////////////////////////////////
float UniquePropagationLayer::calcXPos(const draw::Button* firstButton, const std::size_t x)
{
    float firstButtonPosX{firstButton->getPosition().x};

    return calcPos(firstButton->getSize().width, firstButtonPosX, m_propagationSettings.distanceX, x);
}


////////////////////////////////////////////////////////////
float UniquePropagationLayer::calcYPos(const draw::Button* firstButton, const std::size_t y)
{
    float firstButtonPosY{firstButton->getPosition().y};

    return calcPos(firstButton->getSize().height, firstButtonPosY, m_propagationSettings.distanceY, y);
}


////////////////////////////////////////////////////////////
float UniquePropagationLayer::calcPos(const float buttonDimension, const float firstButtonPosCoord, const float propSettingsDistance, std::size_t iterator)
{
    auto factor = propSettingsDistance >= 0 ? 1 : -1;

    return firstButtonPosCoord + (factor * (buttonDimension * (iterator - 1))) + (propSettingsDistance * (iterator - 1));
}
} // namespace render
} // namespace gfx
} // namespace he