#pragma once

#include "gfx/draw/IShape.hpp"

namespace he
{
namespace gfx
{
namespace geometry::figure
{
class Figure;
} // namespace
namespace draw
{
template<typename POINT, typename VECTOR, typename VERTEX>
class Shape : public draw::IShape<POINT, VECTOR, VERTEX>
{
using IShapeTmpl = he::gfx::draw::IShape<POINT, VECTOR, VERTEX>;

public:
    Shape(const std::string&, const geometry::figures::Figure&);
    Shape(const Shape&, const geometry::figures::Figure&);
    ~Shape() override;

public:
    bool setOrigin(const POINT& origin) override;
    void setOriginInCenter() override;

public:
    void draw(he::gfx::render::Render&, const he::gfx::render::RenderSettings&, render::TransformMatrix&) override;

public:
    bool isPointInside(const POINT& point);
    void setOriginPosition(const he::gfx::OriginPosition& originPosition);
    void closeVertexArray();
    void openVertexArray();

protected:
    void updateVertexArray();

protected:
    const geometry::figures::Figure& m_figure;
    bool m_closedVertexArray{false};
};

using Shape2d = he::gfx::draw::Shape<geometry::Point2Df, geometry::Vector2Df, he::gfx::VertexArray2d>;

} // namespace draw
} // namespace gfx
} // namespace he