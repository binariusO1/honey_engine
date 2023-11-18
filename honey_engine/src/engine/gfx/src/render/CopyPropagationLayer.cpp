#include "gfx/render/CopyPropagationLayer.hpp"

#include <algorithm>
#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace render
{
////////////////////////////////////////////////////////////
CopyPropagationLayer::CopyPropagationLayer(const std::string& name, const PropagationSettings& propagationSettings) 
    : BaseLayer(name)
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
    for (const auto& item : m_uniqueDrawables)
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


////////////////////////////////////////////////////////////
void CopyPropagationLayer::process_event(const he::window::Event& event)
{
    for (auto it = m_uniqueDrawables.begin(); it != m_uniqueDrawables.end(); ++it)
    {
        LOG_WARNING << "Not implemented";
    }
}


////////////////////////////////////////////////////////////
void CopyPropagationLayer::setRenderSettings(const he::gfx::render::RenderSettings& renderSettings)
{
    m_renderSettings = renderSettings;
}


////////////////////////////////////////////////////////////
void CopyPropagationLayer::addDrawable(const std::shared_ptr<he::gfx::draw::IDrawable>& drawable)
{
    m_uniqueDrawables.push_back(drawable);
}


////////////////////////////////////////////////////////////
void CopyPropagationLayer::addDrawables(const DrawableList& drawables)
{
    auto sum = m_uniqueDrawables.size() + drawables.size();

    if (m_uniqueDrawables.capacity() < sum)
    {
        m_uniqueDrawables.reserve(sum);
    }

    m_uniqueDrawables.insert(m_uniqueDrawables.end(), drawables.begin(), drawables.end());
}


////////////////////////////////////////////////////////////
void CopyPropagationLayer::removeDrawable(const std::shared_ptr<he::gfx::draw::IDrawable>& drawable)
{
    auto it = std::find(m_uniqueDrawables.begin(), m_uniqueDrawables.end(), drawable);

    if (it != m_uniqueDrawables.end()) 
    {
        m_uniqueDrawables.erase(it);
    } 
    else 
    {
        LOG_DEBUG << "Element not found.";
    }
}


////////////////////////////////////////////////////////////
he::gfx::draw::IDrawable& CopyPropagationLayer::drawable(const std::string& name)
{
    for (auto& item : m_uniqueDrawables)
    {
        if (item->getName() == name)
        {
            return *item;
        }
    }
    throw he::common::invalid_initialization("Cannot get object. Key does not found");
}

} // namespace render
} // namespace gfx
} // namespace he