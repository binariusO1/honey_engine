#include "gfx/render/CopyPropagationLayer.hpp"

namespace he
{
namespace gfx
{
namespace render
{
////////////////////////////////////////////////////////////
CopyPropagationLayer::CopyPropagationLayer(const std::string& name, const PropagationSettings& propagationSettings) 
    : Layer(name)
    , m_propagationSettings{propagationSettings}
{
}


////////////////////////////////////////////////////////////
CopyPropagationLayer::~CopyPropagationLayer()
{
}


////////////////////////////////////////////////////////////
void CopyPropagationLayer::render(gfx::render::IRender& render)
{
    for (const auto& item : m_shapes)
    {
        auto numX = m_propagationSettings.distanceX > 1 ? m_propagationSettings.numberOfElementsX : 1;
        auto numY = m_propagationSettings.distanceY > 1 ? m_propagationSettings.numberOfElementsY : 1;
        auto startingPosition = item->getPosition();
        for (std::size_t i = 0 ; i < numX; ++i)
        {
            auto xPosition = startingPosition.x + (m_propagationSettings.distanceX*i);
            for (std::size_t i = 0 ; i < numY ; ++i)
            {
                item->setPosition({xPosition, startingPosition.y + (m_propagationSettings.distanceY*i)});
                render.draw(*item, m_renderSettings);
            }
        }
        item->setPosition(startingPosition);
    }
}

} // namespace render
} // namespace gfx
} // namespace he