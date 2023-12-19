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
using Shape2d = he::gfx::draw::Shape<geometry::Point2Df, geometry::Vector2Df, he::gfx::VertexArray2d>;
class ShapeRectangle : public Shape2d
{
public:
    ShapeRectangle(const std::string&, const gfx::geometry::Size2Df&);
    ShapeRectangle(const std::string&, const gfx::geometry::figures::Rectangle&);
    ShapeRectangle(const ShapeRectangle&);
    ~ShapeRectangle() override;

public:
    void setSize(const gfx::geometry::Size2Df& size);
    const gfx::geometry::Size2Df getSize() const;

private:
    gfx::geometry::figures::Rectangle m_rectangle;
};
} // namespace draw
} // namespace gfx
} // namespace he