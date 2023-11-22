#include "gfx/render/Layer.hpp"
#include <algorithm>
#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace render
{
////////////////////////////////////////////////////////////
Layer::Layer(const std::string& name) : BaseLayer(name)
{
}


////////////////////////////////////////////////////////////
Layer::~Layer()
{
}


////////////////////////////////////////////////////////////
void Layer::render(gfx::render::IRender& render)
{
    for (const auto& item : m_uniqueDrawables)
    {
        render.draw(*item);
    }

    for (const auto& layer : m_layers)
    {
        layer.second->render(render);
    }
}


////////////////////////////////////////////////////////////
void Layer::setRenderSettings(const he::gfx::render::RenderSettings& renderSettings)
{
    m_renderSettings = renderSettings;
    for (const auto& layer : m_layers)
    {
        layer.second->setRenderSettings(m_renderSettings);
    }
}


////////////////////////////////////////////////////////////
void Layer::addLayer(const std::shared_ptr<he::gfx::render::ILayer>& layer)
{
    auto result = m_layers.insert({layer->getName(), layer});
    if (not result.second)
    {
        LOG_ERROR << "Unable to add layer: " << layer->getName() << ". Key is already exists";
        return;
    }
}


////////////////////////////////////////////////////////////
void Layer::addLayers(const LayersList& layers)
{
    for (const auto& layer : layers)
    {
        auto result = m_layers.insert({layer->getName(), layer});
        if (not result.second)
        {
            LOG_ERROR << "Unable to add layer: " << layer->getName() << ". Key is already exists";
            return;
        }
    }
}


////////////////////////////////////////////////////////////
void Layer::removeLayer(const std::shared_ptr<he::gfx::render::ILayer>& layer)
{
    auto it = m_layers.find(layer->getName());

    if (it != m_layers.end())
    {
        m_layers.erase(it);
    }
}


////////////////////////////////////////////////////////////
void Layer::addDrawable(const std::shared_ptr<he::gfx::draw::IDrawable>& drawable)
{
    auto it = m_uniqueKeys.emplace(drawable->getName());

    if (it.second)
    {
        m_uniqueDrawables.push_back(drawable);
    }
}


////////////////////////////////////////////////////////////
void Layer::addDrawables(const DrawableList& drawables)
{
    auto sum = m_uniqueDrawables.size() + drawables.size();

    if (m_uniqueDrawables.capacity() < sum)
    {
        m_uniqueDrawables.reserve(sum);
    }

    std::for_each(drawables.begin(), drawables.end(), [&](const std::shared_ptr<he::gfx::draw::IDrawable>& drawable) 
    { 
        auto it = m_uniqueKeys.emplace(drawable->getName());

        if (it.second)
        {
            m_uniqueDrawables.push_back(drawable);
        }
    });
}


////////////////////////////////////////////////////////////
void Layer::removeDrawable(const std::shared_ptr<he::gfx::draw::IDrawable>& drawable)
{
    if (not m_uniqueKeys.erase(drawable->getName()))
    {
        return;
    }

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
he::gfx::render::DrawableList& Layer::drawableList()
{
    return m_uniqueDrawables;
}


////////////////////////////////////////////////////////////
void Layer::process_event(const he::window::Event& event)
{
    for (auto it = m_uniqueListeners.begin(); it != m_uniqueListeners.end(); ++it)
    {
        it->second->process_event(event);
    }
}
} // namespace render
} // namespace gfx
} // namespace he