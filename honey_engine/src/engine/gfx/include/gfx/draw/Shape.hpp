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
class Shape : public IShape<POINT, VECTOR, VERTEX>
{
public:
    Shape(const std::string&, const geometry::figures::Figure&);
    Shape(const Shape&, const geometry::figures::Figure&);
    ~Shape() override;

public:
    bool setOrigin(const POINT& origin) override;
    void setOriginInCenter() override;

public:
    void draw(render::Render&, const render::RenderSettings&, render::TransformMatrix&) override;

public:
    // TODO : point inside a punkt w obrysie na view X,Y to dwie różne rzeczy
    // dla 3d: jeśli obrócę w 3d prostokąt w osi X lub Y, to na ekranie będzie trapezem
    // isPointInside ma sens wtedy, gdy zbierany jest punkt X,Y dla MouseEvent, dlatego dla 3d potrzebna jest oddzielna specjalizacja
    bool isPointInside(const geometry::Point2Df& point);
    void closeVertexArray();
    void openVertexArray();

protected:
    void updateVertexArray();

protected:
    const geometry::figures::Figure& m_figure;
    bool m_closedVertexArray{false};

    using IShapeTmpl = IShape<POINT, VECTOR, VERTEX>;
};

using Shape2d = Shape<geometry::Point2Df, geometry::Vector2Df, VertexArray2d>;
using Shape2dFor3d = Shape<geometry::Point3Df, geometry::Vector3Df, VertexArray3d>;

} // namespace draw
} // namespace gfx
} // namespace he