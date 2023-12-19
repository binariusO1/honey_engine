#include "gfx/render/BaseLayer.hpp"

#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace render
{
////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
BaseLayer<POINT, VECTOR>::BaseLayer(const std::string& name) : m_context(name)
{
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
BaseLayer<POINT, VECTOR>::~BaseLayer()
{
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
void BaseLayer<POINT, VECTOR>::show()
{
    m_context.visible = VisibilityState::visible;
    LOG_DEBUG << "Layer: " << this->getName() << " is visible";
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
void BaseLayer<POINT, VECTOR>::hide()
{
    m_context.visible = VisibilityState::hidden;
    LOG_DEBUG << "Layer: " << this->getName() << " is hidden";
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
bool BaseLayer<POINT, VECTOR>::isVisible() const
{
    return (m_context.visible == VisibilityState::visible);
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR>
const std::string BaseLayer<POINT, VECTOR>::getName() const
{
    return m_context.name;
}

template class BaseLayer<geometry::Point2Df, geometry::Vector2Df>;
template class BaseLayer<geometry::Point3Df, geometry::Vector3Df>;

} // namespace render
} // namespace gfx
} // namespace he