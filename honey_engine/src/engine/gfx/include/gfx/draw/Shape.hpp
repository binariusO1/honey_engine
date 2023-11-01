#pragma once

#include <memory>
#include "gfx/Vertex.hpp"
#include "gfx/draw/IDrawable.hpp"
#include "gfx/geometry/figures/Figure.hpp"
#include "gfx/geometry/transform/Transformable2d.hpp"

namespace he
{
namespace gfx
{
namespace draw
{

class Shape : public IDrawable , protected he::gfx::geometry::transform::Transformable2d
{
public:
    Shape(const std::string&, const std::shared_ptr<he::gfx::geometry::figures::Figure>&);
    Shape(const std::string&, 
            const std::shared_ptr<he::gfx::geometry::figures::Figure>&, 
            const he::gfx::Color&, 
            const he::gfx::geometry::Point2Df&,
            const he::gfx::OriginPosition&);
    ~Shape() override;

public:
    bool isPointInside(const geometry::Point2Df& point);
    void setColor(const he::gfx::Color& color) override;
    void setOrigin(const he::gfx::geometry::Point2Df& origin) override;
    void setOriginInCenter() override;
    void setPosition(const he::gfx::geometry::Point2Df& position) override;
    void setRotation(const he::gfx::geometry::Angle& angle) override;
    he::gfx::Color getColor() const override;
    const std::string getName() const override;
    const he::gfx::geometry::Point2Df& getOrigin() const override;
    const he::gfx::geometry::Point2Df& getPosition() const override;
    const he::gfx::geometry::Angle& getRotation() const override;
    unsigned int getTextureId() const override;

public:
    const he::gfx::VertexArray2d& getVertexArray() const override;
    void closeVertexArray();
    void openVertexArray();

protected:
    void setOriginPosition(const he::gfx::OriginPosition& originPosition);
    void updateVertexArray();

protected:
    void draw(he::gfx::render::Render&, const he::gfx::render::RenderSettings&) const override;

protected:
    const std::shared_ptr<he::gfx::geometry::figures::Figure> m_figure{nullptr};
    he::gfx::VertexArray2d m_vertexArray{};
    IDrawable::Context m_context;
    bool m_closedVertexArray{false};
};
} // namespace draw
} // namespace gfx
} // namespace he