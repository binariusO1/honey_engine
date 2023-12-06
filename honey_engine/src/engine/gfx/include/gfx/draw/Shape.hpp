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
    Shape(const std::string&, const std::shared_ptr<geometry::figures::Figure>);
    Shape(const Shape&);
    ~Shape() override;

public:
    void setColor(const he::gfx::Color& color) override;
    void setPosition(const he::gfx::geometry::Point2Df& position) override;
    const he::gfx::Color getColor() const override;
    const he::gfx::geometry::Point2Df& getPosition() const override;
    const std::string getName() const override;
    const he::gfx::VertexArray2d& getVertexArray() const override;

public:
    void setOrigin(const he::gfx::geometry::Point2Df& origin) override;
    const he::gfx::geometry::Point2Df& getOrigin() const override;
    void setRotation(const he::gfx::geometry::Angle& angle) override;
    const he::gfx::geometry::Angle& getRotation() const override;
    bool isPointInside(const gfx::geometry::Point2Df& point);
    void setOriginInCenter() override;

public:
    void draw(he::gfx::render::Render&, const he::gfx::render::RenderSettings&) override;

public:
    void setOriginPosition(const he::gfx::OriginPosition& originPosition);
    void closeVertexArray();
    void openVertexArray();
    void update();

protected:
    void updateVertexArray();

protected:
    const std::shared_ptr<geometry::figures::Figure> m_figure{nullptr};
    bool m_closedVertexArray{false};
};
} // namespace draw
} // namespace gfx
} // namespace he