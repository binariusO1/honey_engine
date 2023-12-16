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
    // TODO 2: improvement
    // note: to reduce number of drawVertex2d in Render.cpp updates
    // 1_ create structure like <item, copies_of_item>
    // 2_ first time set position for all
    // 3_ if no change with position of first element, just draw cached vertexArray
    for (const auto& item : m_shapes)
    {
        auto numX = m_propagationSettings.numberOfElementsX > 1 ? m_propagationSettings.numberOfElementsX : 1;
        auto numY = m_propagationSettings.numberOfElementsY > 1 ? m_propagationSettings.numberOfElementsY : 1;
        auto startingPosition = item->getPosition();
        for (std::size_t i = 0 ; i < numX; ++i)
        {
            auto xPosition = startingPosition.x + (m_propagationSettings.distanceX * i);
            for (std::size_t j = 0 ; j < numY ; ++j)
            {
                item->setPosition({xPosition, startingPosition.y + (m_propagationSettings.distanceY * j)});
                render.draw(*item, m_renderSettings, m_transformMatrix);
            }
        }
        item->setPosition(startingPosition);
    }
}

} // namespace render
} // namespace gfx
} // namespace he