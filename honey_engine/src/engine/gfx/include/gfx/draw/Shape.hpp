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
using IShape2d = he::gfx::draw::IShape<geometry::Point2Df, geometry::Vector2Df, he::gfx::VertexArray2d>;

//template<typename POINT, typename VECTOR, typename VERTEX>
class Shape : public IShape2d//he::gfx::draw::IShape<POINT, VECTOR, VERTEX>
{
public:
    Shape(const std::string&, const geometry::figures::Figure&);
    Shape(const Shape&, const geometry::figures::Figure&);
    ~Shape() override;

public:
    bool setOrigin(const he::gfx::geometry::Point2Df& origin) override;
    void setOriginInCenter() override;

public:
    void draw(he::gfx::render::Render&, const he::gfx::render::RenderSettings&, render::TransformMatrix&) override;

public:
    bool isPointInside(const gfx::geometry::Point2Df& point);
    void setOriginPosition(const he::gfx::OriginPosition& originPosition);
    void closeVertexArray();
    void openVertexArray();

protected:
    void updateVertexArray();

protected:
    const geometry::figures::Figure& m_figure;
    bool m_closedVertexArray{false};
};
} // namespace draw
} // namespace gfx
} // namespace he