#include "gfx/render/Layer.hpp"

#include <algorithm>
#include "gfx/render/TransformMatrix.hpp"
#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace render
{
////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
Layer<POINT, VECTOR, VERTEX>::Layer(const std::string& name) : BaseLayerTmpl(name)
{
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
Layer<POINT, VECTOR, VERTEX>::Layer::~Layer()
{
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::render(gfx::render::IRender& render)
{
    for (const auto& item : m_buttons)
    {
        render.draw(*item, BaseLayerTmpl::m_transformMatrix);
    }

    for (const auto& item : m_shapes)
    {
        render.draw(*item, BaseLayerTmpl::m_transformMatrix);
    }

    for (const auto& layer : m_layers)
    {
        layer.second->render(render); //todo : pass transformMatrix
    }
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::setRenderSettings(const he::gfx::render::RenderSettings& renderSettings)
{
    BaseLayerTmpl::m_renderSettings = renderSettings;
    for (const auto& layer : m_layers)
    {
        layer.second->setRenderSettings(BaseLayerTmpl::m_renderSettings);
    }
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
bool Layer<POINT, VECTOR, VERTEX>::setPosition(const POINT& position)
{
    auto result = BaseLayerTmpl::setPosition(position);

    BaseLayerTmpl::m_transformMatrix.viewMatrix = BaseLayerTmpl::getTransform().getMatrix();
    BaseLayerTmpl::m_transformMatrix.isNeedUpdate = true;

    return result;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::addLayer(const std::shared_ptr<he::gfx::render::ILayer>& layer)
{
    auto result = m_layers.insert({layer->getName(), layer});
    if (not result.second)
    {
        LOG_ERROR << "Unable to add layer: " << layer->getName() << ". Key is already exists";
        return;
    }
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::addLayers(const LayersList& layers)
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
template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::removeLayer(const std::shared_ptr<he::gfx::render::ILayer>& layer)
{
    auto it = m_layers.find(layer->getName());

    if (it != m_layers.end())
    {
        m_layers.erase(it);
    }
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::addShape(const std::shared_ptr<IShapeTmpl>& shape)
{
    auto it = m_keys.emplace(shape->getName());

    if (it.second)
    {
        m_shapes.push_back(shape);
        m_shapes.back()->setLayerName(BaseLayerTmpl::m_context.name);
    }
    else
    {
        LOG_WARNING << "Unable to add shape: " << shape->getName() << ". Key is already exists";
    }
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::addShapes(const IShapeListTmpl& shapes)
{
    auto sum = m_shapes.size() + shapes.size();

    if (m_shapes.capacity() < sum)
    {
        m_shapes.reserve(sum);
    }

    std::for_each(shapes.begin(), shapes.end(), [&](const std::shared_ptr<IShapeTmpl>& shape) 
    { 
        auto it = m_keys.emplace(shape->getName());

        if (it.second)
        {
            m_shapes.push_back(shape);
            m_shapes.back()->setLayerName(BaseLayerTmpl::m_context.name);
        }
        else
        {
            LOG_WARNING << "Unable to add shape: " << shape->getName() << ". Key is already exists";
        }
    });
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::removeShape(const std::shared_ptr<IShapeTmpl>& shape)
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
template<typename POINT, typename VECTOR, typename VERTEX> 
std::vector<std::shared_ptr<draw::IShape<POINT, VECTOR, VERTEX>>>& Layer<POINT, VECTOR, VERTEX>::getShapeList()
{
    return m_shapes;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
draw::IShape<POINT, VECTOR, VERTEX>& Layer<POINT, VECTOR, VERTEX>::shape(const std::string& name)
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
template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::addButton(const std::shared_ptr<gfx::draw::Button>& button)
{
    auto it = m_keys.emplace(button->getName());

    if (it.second)
    {
        m_buttons.push_back(button);
        m_buttons.back()->setLayerName(BaseLayerTmpl::m_context.name);
    }
    else
    {
        LOG_WARNING << "Unable to add button: " << button->getName() << ". Key is already exists";
    }
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::addButtons(const ButtonList& buttons)
{
    auto sum = m_buttons.size() + buttons.size();

    if (m_buttons.capacity() < sum)
    {
        m_buttons.reserve(sum);
    }

    std::for_each(buttons.begin(), buttons.end(), [&](const std::shared_ptr<he::gfx::draw::Button>& button) 
    { 
        auto it = m_keys.emplace(button->getName());

        if (it.second)
        {
            m_buttons.push_back(button);
            m_buttons.back()->setLayerName(BaseLayerTmpl::m_context.name);
        }
        else
        {
            LOG_WARNING << "Unable to add button: " << button->getName() << ". Key is already exists";
        }
    });
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
he::gfx::render::ButtonList& Layer<POINT, VECTOR, VERTEX>::getButtons()
{
    return m_buttons;
}


////////////////////////////////////////////////////////////

template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::process_event(const he::window::Event& event)
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
template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::onMouseButtonPressed(const he::window::Event::MouseButtonAction& event)
{
    window::Event::MouseButtonAction adjEvent = event;
    adjustPointsForEvent(adjEvent.x, adjEvent.y);

    for (const auto& button : m_buttons)
    {
        if (button->onMouseButtonPressed(adjEvent) and m_firstonMouseButtonPressed)
        {
            return;
        }
    }
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::onMouseButtonReleased(const he::window::Event::MouseButtonAction& event)
{
    window::Event::MouseButtonAction adjEvent = event;
    adjustPointsForEvent(adjEvent.x, adjEvent.y);

    for (const auto& button : m_buttons)
    {
        if (button->onMouseButtonReleased(adjEvent) and m_firstonMouseButtonPressed)
        {
            return;
        }
    }
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::onMouseCursorMoved(const he::window::Event::MouseMoveEvent& event)
{
    window::Event::MouseMoveEvent adjEvent = event;
    adjustPointsForEvent(adjEvent.x, adjEvent.y);

    for (const auto& button : m_buttons)
    {
        if (button->onMouseCursorMoved(adjEvent) and m_firstonMouseButtonPressed)
        {
            return;
        }
    }
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> 
void Layer<POINT, VECTOR, VERTEX>::adjustPointsForEvent(int& x, int& y)
{
    he::gfx::geometry::Point2Df pointToCheck{static_cast<float>(x), static_cast<float>(y)};
    BaseLayerTmpl::inverseTransformPoint(pointToCheck);
    x = static_cast<int>(pointToCheck.x);
    y = static_cast<int>(pointToCheck.y);
}

template class Layer<geometry::Point2Df, geometry::Vector2Df, VertexArray2d>;
template class Layer<geometry::Point3Df, geometry::Vector3Df, VertexArray3d>;

} // namespace render
} // namespace gfx
} // namespace he