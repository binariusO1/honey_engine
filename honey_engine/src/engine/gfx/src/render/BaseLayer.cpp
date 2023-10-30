#include "gfx/render/BaseLayer.hpp"

#include "logger/Logger.hpp"

namespace he
{
namespace gfx
{
namespace render
{
////////////////////////////////////////////////////////////
BaseLayer::BaseLayer(const std::string& name) : m_context(name)
{
}


////////////////////////////////////////////////////////////
BaseLayer::~BaseLayer()
{
}


////////////////////////////////////////////////////////////
void BaseLayer::show()
{
    m_context.visible = VisibilityState::visible;
    LOG_DEBUG << "Layer: " << this->getName() << " is visible";
}


////////////////////////////////////////////////////////////
void BaseLayer::hide()
{
    m_context.visible = VisibilityState::hidden;
    LOG_DEBUG << "Layer: " << this->getName() << " is hidden";
}


////////////////////////////////////////////////////////////
bool BaseLayer::isVisible() const
{
    return (m_context.visible == VisibilityState::visible);
}


////////////////////////////////////////////////////////////
const std::string BaseLayer::getName() const
{
    return m_context.name;
}
} // namespace render
} // namespace gfx
} // namespace he