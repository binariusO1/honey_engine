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

class Shape : public IShape
{
public:
    Shape(const std::string&, const geometry::figures::Figure&);
    Shape(const Shape&, const geometry::figures::Figure&);
    ~Shape() override;

public:
    void setColor(const he::gfx::Color& color) override;
    bool setPosition(const he::gfx::geometry::Point2Df& position) override;
    const he::gfx::Color getColor() const override;
    const he::gfx::geometry::Point2Df& getPosition() const override;
    const std::string getName() const override;
    const he::gfx::VertexArray2d& getVertexArray() const override;

public:
    bool setOrigin(const he::gfx::geometry::Point2Df& origin) override;
    const he::gfx::geometry::Point2Df& getOrigin() const override;
    bool setRotation(const he::gfx::geometry::Angle& angle, const int axis = 0) override;
    const he::gfx::geometry::Angle& getRotation(const int axis = 0) const override;
    bool isPointInside(const gfx::geometry::Point2Df& point);
    void setOriginInCenter() override;

public:
    void draw(he::gfx::render::Render&, const he::gfx::render::RenderSettings&, render::TransformMatrix&) override;

public:
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