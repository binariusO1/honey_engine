#pragma once

#include <memory>
#include "gfx/Vertex.hpp"
#include "gfx/draw/Drawable.hpp"
#include "gfx/geometry/figures/Rectangle.hpp"
#include "gfx/geometry/transform/Transformable2d.hpp"

namespace he
{
namespace gfx
{
namespace draw
{

class Shape : public Drawable , protected he::gfx::geometry::transform::Transformable2d
{
public:
    Shape(const std::string&, const std::shared_ptr<he::gfx::geometry::figures::Figure>&);
    Shape(const std::string&, const he::gfx::geometry::figures::Rectangle&);
    Shape(const std::string&, 
            const std::shared_ptr<he::gfx::geometry::figures::Figure>&, 
            const he::gfx::Color&, 
            const he::gfx::geometry::Point2Df&,
            const he::gfx::OriginPosition&);
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
    bool isPointInside(const geometry::Point2Df& point);
    void setOriginInCenter();

public:
    void draw(he::gfx::render::Render&, const he::gfx::render::RenderSettings&) override;

public:
    void closeVertexArray();
    void openVertexArray();
    void update();

protected:
    void setOriginPosition(const he::gfx::OriginPosition& originPosition);
    void updateVertexArray();

protected:
    const std::shared_ptr<he::gfx::geometry::figures::Figure> m_figure{nullptr};
    he::gfx::VertexArray2d m_vertexArray{};
    Drawable::Context m_context;
    bool m_closedVertexArray{false};
    bool m_vertexArrayNeedUpdate{false};
};
} // namespace draw
} // namespace gfx
} // namespace he