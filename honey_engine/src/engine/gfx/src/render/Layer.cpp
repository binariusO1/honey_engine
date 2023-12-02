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
    for (const auto& item : m_shapes)
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
void Layer::addShape(const std::shared_ptr<he::gfx::draw::IShape>& drawable)
{
    auto it = m_keys.emplace(drawable->getName());

    if (it.second)
    {
        m_shapes.push_back(drawable);
    }
    else
    {
        LOG_WARNING << "Unable to add drawable: " << drawable->getName() << ". Key is already exists";
    }
}


////////////////////////////////////////////////////////////
void Layer::addShapes(const ShapeList& drawables)
{
    auto sum = m_shapes.size() + drawables.size();

    if (m_shapes.capacity() < sum)
    {
        m_shapes.reserve(sum);
    }

    std::for_each(drawables.begin(), drawables.end(), [&](const std::shared_ptr<he::gfx::draw::IShape>& drawable) 
    { 
        auto it = m_keys.emplace(drawable->getName());

        if (it.second)
        {
            m_shapes.push_back(drawable);
        }
        else
        {
            LOG_WARNING << "Unable to add drawable: " << drawable->getName() << ". Key is already exists";
        }
    });
}


////////////////////////////////////////////////////////////
void Layer::removeShape(const std::shared_ptr<he::gfx::draw::IShape>& drawable)
{
    if (not m_keys.erase(drawable->getName()))
    {
        return;
    }

    auto it = std::find(m_shapes.begin(), m_shapes.end(), drawable);

    if (it != m_shapes.end()) 
    {
        m_shapes.erase(it);       
    } 
    else 
    {
        LOG_DEBUG << "Element not found.";
    }
}


////////////////////////////////////////////////////////////
he::gfx::render::ShapeList& Layer::shapeList()
{
    return m_shapes;
}


////////////////////////////////////////////////////////////
he::gfx::draw::IShape& Layer::shape(const std::string& name)
{
    for (auto& item : m_shapes)
    {
        if (item->getName() == name)
        {
            return *item;
        }
    }
    throw he::common::invalid_initialization("Cannot get object. Key does not found");
}


////////////////////////////////////////////////////////////
void Layer::addButton(const std::shared_ptr<gfx::draw::IButton>& button)
{
    m_buttons.push_back(button);
}


////////////////////////////////////////////////////////////
void Layer::process_event(const he::window::Event& event)
{
    // LOG_DEBUG << "Process event: " << window::toString(event.type) << ", layer name: " << m_context.name;

    switch (event.type)
    {
        case window::Event::EventType::mouseButtonPressed:
            onMauseButtonPressed(event.mouseButton);
            return;
        default:
            break;
    }
}


////////////////////////////////////////////////////////////
void Layer::onMauseButtonPressed(const he::window::Event::MouseButtonAction& event)
{
    for (const auto& button : m_buttons)
    {
        if (button->onMauseButtonPressed(event) and m_firstOnMauseButtonPressed)
        {
            return;
        }
    }
}

} // namespace render
} // namespace gfx
} // namespace he