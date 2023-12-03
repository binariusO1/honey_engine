#include "gfx/render/UniquePropagationLayer.hpp"

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
    const auto numX = m_propagationSettings.numberOfElementsX > 1 ? m_propagationSettings.numberOfElementsX : 1;
    const auto numY = m_propagationSettings.numberOfElementsY > 1 ? m_propagationSettings.numberOfElementsY : 1;
    const auto firstButtonPosition = button->getPosition();
    float xPosition{0.f};
    float yPosition{0.f};
    std::size_t x{1};
    std::size_t y{1};

    Layer::addButton(button);

    for (; x <= numX; ++x)
    {
        xPosition = firstButtonPosition.x + (button->getSize().width * (x - 1)) + (m_propagationSettings.distanceX * x);

        for (; y <= numY ; ++y)
        {
            yPosition = firstButtonPosition.y + (button->getSize().height * (y - 1)) + (m_propagationSettings.distanceY * y);

            if (x > 1 or (x == 1 and y > 1))
            {
                addNewButton(button.get(), {xPosition, yPosition}, {x, y});
            }
        }
        y = 1;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UniquePropagationLayer::addNewButton(const draw::Button* button, const gfx::geometry::Point2Df position, const gfx::geometry::Point2Dt coords)
{
    std::shared_ptr<he::gfx::draw::Button> newButton = std::make_shared<he::gfx::draw::Button>(*button);
    newButton->setPosition({position.x, position.y});
    newButton->setName(newButton->getName() + "_x" + std::to_string(coords.x) + "_y" + std::to_string(coords.y));
    Layer::addButton(newButton);
}
} // namespace render
} // namespace gfx
} // namespace he