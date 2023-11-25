#include "gfx/draw/ShapeRectangle.hpp"

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
    , Shape(copy)
{
}


////////////////////////////////////////////////////////////
ShapeRectangle::~ShapeRectangle() = default;


////////////////////////////////////////////////////////////
void ShapeRectangle::setSize(const gfx::geometry::Size2Df& size)
{
    m_rectangle.setSize(size);
    m_vertexArrayNeedUpdate = true;
}

} // namespace draw
} // namespace gfx
} // namespace he