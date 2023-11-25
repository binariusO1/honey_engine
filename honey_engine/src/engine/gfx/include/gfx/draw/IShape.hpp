#pragma once

#include <memory>
#include "gfx/draw/Drawable.hpp"
#include "gfx/geometry/figures/Rectangle.hpp"
#include "gfx/geometry/transform/Transformable2d.hpp"

namespace he
{
namespace gfx
{
namespace draw
{

class IShape : public Drawable , protected he::gfx::geometry::transform::Transformable2d
{
protected:
    IShape(const std::string&);
    IShape(const IShape&);

public:
    ~IShape() override;

public:
    void setColor(const he::gfx::Color& color) override;
    const he::gfx::Color getColor() const override;
    void setPosition(const he::gfx::geometry::Point2Df& position) override;
    const he::gfx::geometry::Point2Df& getPosition() const override;
    const std::string getName() const override;
    const he::gfx::VertexArray2d& getVertexArray() const override;

public:
    void setOrigin(const he::gfx::geometry::Point2Df& point) override;
    const he::gfx::geometry::Point2Df& getOrigin() const override;
    void setRotation(const he::gfx::geometry::Angle& angle) override;
    const he::gfx::geometry::Angle& getRotation() const override;

public:
    virtual void setOriginInCenter() = 0;
    void setOriginPosition(const he::gfx::OriginPosition& originPosition);
    void update();
    void checkIfVertexArrayNeedUpdateThenUpdate();

protected:
    virtual void updateVertexArray() = 0;

protected:
    he::gfx::VertexArray2d m_vertexArray{};
    Drawable::Context m_context;
    bool m_vertexArrayNeedUpdate{false};
};
} // namespace draw
} // namespace gfx
} // namespace he