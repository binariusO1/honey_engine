#include "gfx/render/CopyPropagationLayer.hpp"

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
    for (auto it = m_uniqueDrawables.begin(); it != m_uniqueDrawables.end(); ++it)
    {
        auto numX = m_propagationSettings.distanceX > 1 ? m_propagationSettings.numberOfElementsX : 1;
        auto numY = m_propagationSettings.distanceY > 1 ? m_propagationSettings.numberOfElementsY : 1;
        auto startingPosition = it->second->getPosition();
        for (std::size_t i = 0 ; i < numX; ++i)
        {
            auto xPosition = startingPosition.x + (m_propagationSettings.distanceX*i);
            for (std::size_t i = 0 ; i < numY ; ++i)
            {
                it->second->setPosition({xPosition, startingPosition.y + (m_propagationSettings.distanceY*i)});
                render.draw(*(it->second), m_renderSettings);
            }
        }
        it->second->setPosition(startingPosition);
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
    auto result = m_uniqueDrawables.insert({drawable->getName(), drawable});
    if (not result.second)
    {
        LOG_ERROR << "Unable to add drawable: " << drawable->getName() << ". Key is already exists";
        return;
    }
}


////////////////////////////////////////////////////////////
void CopyPropagationLayer::addDrawables(const DrawableList& drawables)
{
    for (const auto& drawable : drawables)
    {
        auto result = m_uniqueDrawables.insert({drawable->getName(), drawable});
        if (not result.second)
        {
            LOG_ERROR << "Unable to add drawable: " << drawable->getName() << ". Key is already exists";
            return;
        }
    }
}


////////////////////////////////////////////////////////////
void CopyPropagationLayer::removeDrawable(const std::shared_ptr<he::gfx::draw::IDrawable>& drawable)
{
    auto it = m_uniqueDrawables.find(drawable->getName());
    if (it != m_uniqueDrawables.end())
    {
        m_uniqueDrawables.erase(it);
    }
}


////////////////////////////////////////////////////////////
he::gfx::draw::IDrawable& CopyPropagationLayer::drawable(const std::string& name)
{
    auto it = m_uniqueDrawables.find(name);
    if (it != m_uniqueDrawables.end())
    {
        return *(it->second);
    }
    else
    {
        throw he::common::invalid_initialization("Cannot get object. Key does not found");
    }
}

} // namespace render
} // namespace gfx
} // namespace he