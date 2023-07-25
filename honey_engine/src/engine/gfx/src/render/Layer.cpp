#include "gfx/render/Layer.hpp"
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
    for (auto it = m_uniqueDrawables.begin(); it != m_uniqueDrawables.end(); ++it)
    {
        render.draw(*(it->second), m_renderSettings);
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
    auto result = m_uniqueDrawables.insert({drawable->getName(), drawable});
    if (not result.second)
    {
        LOG_ERROR << "Unable to add drawable: " << drawable->getName() << ". Key is already exists";
        return;
    }
}


////////////////////////////////////////////////////////////
void Layer::addDrawables(const DrawableList& drawables)
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
void Layer::removeDrawable(const std::shared_ptr<he::gfx::draw::IDrawable>& drawable)
{
    auto it = m_uniqueDrawables.find(drawable->getName());
    if (it != m_uniqueDrawables.end())
    {
        m_uniqueDrawables.erase(it);
    }
}


////////////////////////////////////////////////////////////
he::gfx::draw::IDrawable& Layer::drawable(const std::string& name)
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