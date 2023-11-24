#pragma once

#include "gfx/draw/Shape.hpp"
#include "gfx/geometry/figures/Rectangle.hpp"
// TODO UT
namespace he
{
namespace gfx
{
namespace draw
{

class ShapeRectangle : public Shape
{
public:
    ShapeRectangle(const std::string&, const gfx::geometry::Size2Df&);
    ShapeRectangle(const std::string&, const gfx::geometry::figures::Rectangle&);
    ShapeRectangle(const ShapeRectangle&);
    ~ShapeRectangle() override;

public:
    void setSize(const gfx::geometry::Size2Df& size);

private:
    gfx::geometry::figures::Rectangle m_rectangle;
};
} // namespace draw
} // namespace gfx
} // namespace he