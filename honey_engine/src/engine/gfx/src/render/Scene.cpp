#include "gfx/render/Scene.hpp"

#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace render
{
////////////////////////////////////////////////////////////
Scene::Scene(const std::string& name) : m_context(name)
{
    m_context.state = OpenCloseState::closed;
}

////////////////////////////////////////////////////////////
Scene::~Scene()
{
}


////////////////////////////////////////////////////////////
void Scene::open()
{
    m_context.state = OpenCloseState::open;
    LOG_DEBUG << "Scene: " << this->getName() << " is open";
}


////////////////////////////////////////////////////////////
void Scene::close()
{
    m_context.state = OpenCloseState::closed;
    LOG_DEBUG << "Scene: " << this->getName() << " is closed";
}


////////////////////////////////////////////////////////////
bool Scene::isOpen() const
{
    return (m_context.state == OpenCloseState::open);
}


////////////////////////////////////////////////////////////
void Scene::addLayer(const std::shared_ptr<he::gfx::render::ILayer>& layer)
{
    auto result = m_layers.insert({layer->getName(), layer});
    if (not result.second)
    {
        throw he::common::invalid_initialization("Unable to add transition table! Key is already exists");
        return;
    }
}


////////////////////////////////////////////////////////////
void Scene::addLayers(const LayersList& layers)
{
    for (const auto& layer : layers)
    {
        auto result = m_layers.insert({layer->getName(), layer});
        if (not result.second)
        {
            throw he::common::invalid_initialization("Unable to add transition table! Key is already exists");
            return;
        }
    }
}


////////////////////////////////////////////////////////////
void Scene::removeLayer(const std::shared_ptr<he::gfx::render::ILayer>& layer)
{
    auto it = m_layers.find(layer->getName());
    if (it != m_layers.end())
    {
        m_layers.erase(it);
    }
}


////////////////////////////////////////////////////////////
void Scene::render(gfx::render::IRender& render)
{
    for (const auto& layer : m_layers)
    {
        if (layer.second->isVisible())
        {
            layer.second->render(render);
        }
    }
}


////////////////////////////////////////////////////////////
void Scene::process_event(const he::window::Event& event)
{
    for (const auto& layer : m_layers)
    {
        if (layer.second->isVisible())
        {
            layer.second->process_event(event);
        }
    }
}


////////////////////////////////////////////////////////////
const std::string Scene::getName()
{
    return m_context.name;
}


} // namespace render
} // namespace gfx
} // namespace he