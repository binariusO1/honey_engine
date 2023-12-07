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
    for (const auto& item : m_buttons)
    {
        render.draw(*item);
    }

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
void Layer::addShape(const std::shared_ptr<he::gfx::draw::IShape>& shape)
{
    auto it = m_keys.emplace(shape->getName());

    if (it.second)
    {
        m_shapes.push_back(shape);
        m_shapes.back()->setLayerName(m_context.name);
    }
    else
    {
        LOG_WARNING << "Unable to add shape: " << shape->getName() << ". Key is already exists";
    }
}


////////////////////////////////////////////////////////////
void Layer::addShapes(const ShapeList& shape)
{
    auto sum = m_shapes.size() + shape.size();

    if (m_shapes.capacity() < sum)
    {
        m_shapes.reserve(sum);
    }

    std::for_each(shape.begin(), shape.end(), [&](const std::shared_ptr<he::gfx::draw::IShape>& shape) 
    { 
        auto it = m_keys.emplace(shape->getName());

        if (it.second)
        {
            m_shapes.push_back(shape);
            m_shapes.back()->setLayerName(m_context.name);
        }
        else
        {
            LOG_WARNING << "Unable to add shape: " << shape->getName() << ". Key is already exists";
        }
    });
}


////////////////////////////////////////////////////////////
void Layer::removeShape(const std::shared_ptr<he::gfx::draw::IShape>& shape)
{
    if (not m_keys.erase(shape->getName()))
    {
        return;
    }

    auto it = std::find(m_shapes.begin(), m_shapes.end(), shape);

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
void Layer::addButton(const std::shared_ptr<gfx::draw::Button>& button)
{
    auto it = m_keys.emplace(button->getName());

    if (it.second)
    {
        m_buttons.push_back(button);
        m_buttons.back()->setLayerName(m_context.name);
    }
    else
    {
        LOG_WARNING << "Unable to add button: " << button->getName() << ". Key is already exists";
    }
}


////////////////////////////////////////////////////////////
he::gfx::render::ButtonList& Layer::getButtons()
{
    return m_buttons;
}


////////////////////////////////////////////////////////////
void Layer::process_event(const he::window::Event& event)
{
    //  LOG_DEBUG << "Process event: " << window::toString(event.type) << ", layer name: " << m_context.name;

    switch (event.type)
    {
        case window::Event::EventType::mouseButtonPressed:
            onMouseButtonPressed(event.mouseButton);
            return;
        case window::Event::EventType::mouseButtonReleased:
            onMouseButtonReleased(event.mouseButton);
            return;
        case window::Event::EventType::mouseCursorMoved:
            onMouseCursorMoved(event.mouseMove);
            return;
        default:
            break;
    }
}


////////////////////////////////////////////////////////////
void Layer::onMouseButtonPressed(const he::window::Event::MouseButtonAction& event)
{
    for (const auto& button : m_buttons)
    {
        if (button->onMouseButtonPressed(event) and m_firstonMouseButtonPressed)
        {
            return;
        }
    }
}


////////////////////////////////////////////////////////////
void Layer::onMouseButtonReleased(const he::window::Event::MouseButtonAction& event)
{
    for (const auto& button : m_buttons)
    {
        if (button->onMouseButtonReleased(event) and m_firstonMouseButtonPressed)
        {
            return;
        }
    }
}


////////////////////////////////////////////////////////////
void Layer::onMouseCursorMoved(const he::window::Event::MouseMoveEvent& event)
{
    for (const auto& button : m_buttons)
    {
        if (button->onMouseCursorMoved(event) and m_firstonMouseButtonPressed)
        {
            return;
        }
    }
}
} // namespace render
} // namespace gfx
} // namespace he