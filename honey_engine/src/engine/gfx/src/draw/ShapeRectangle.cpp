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
ShapeRectangle::ShapeRectangle(const std::string& name, const geometry::Size2Df& size) : m_rectangle{size} , Shape(name, std::make_shared<geometry::figures::Rectangle>(size)) 
{
}


////////////////////////////////////////////////////////////
ShapeRectangle::ShapeRectangle(const std::string& name, const gfx::geometry::figures::Rectangle& rect)
    : Shape(name, std::make_shared<geometry::figures::Rectangle>(rect))
    , m_rectangle{rect} 
{
}


////////////////////////////////////////////////////////////
ShapeRectangle::ShapeRectangle(const ShapeRectangle& copy)
    : Shape(copy)
    , m_rectangle{copy.m_rectangle}
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


////////////////////////////////////////////////////////////
const gfx::geometry::Size2Df ShapeRectangle::getSize() const
{
    return m_rectangle.getSize();
}


} // namespace draw
} // namespace gfx
} // namespace he