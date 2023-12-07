#include "gfx/draw/ShapeRectangle.hpp"
#include "logger/Logger.hpp"
namespace 
{
} // namespace
namespace he
{
namespace gfx
{
namespace draw
{
////////////////////////////////////////////////////////////
ShapeRectangle::ShapeRectangle(const std::string& name, const geometry::Size2Df& size) : m_rectangle(size) , Shape(name, m_rectangle)
{
}


////////////////////////////////////////////////////////////
ShapeRectangle::ShapeRectangle(const std::string& name, const gfx::geometry::figures::Rectangle& rect)
    : m_rectangle{rect} 
    , Shape(name, m_rectangle)
{
}


////////////////////////////////////////////////////////////
ShapeRectangle::ShapeRectangle(const ShapeRectangle& copy)
    : m_rectangle{copy.m_rectangle}
    , Shape(copy, m_rectangle)
{
    //m_figure = static_cast<gfx::geometry::figures::Figure&>(m_rectangle);
}


////////////////////////////////////////////////////////////
ShapeRectangle::~ShapeRectangle() = default;


////////////////////////////////////////////////////////////
void ShapeRectangle::setSize(const gfx::geometry::Size2Df& size)
{
    m_rectangle.setSize(size);
    m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
const gfx::geometry::Size2Df ShapeRectangle::getSize() const
{
    return m_rectangle.getSize();
}


} // namespace draw
} // namespace gfx
} // namespace he