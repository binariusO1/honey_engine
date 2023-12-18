#pragma once

#include <memory>
#include "gfx/draw/Drawable.hpp"
#include "gfx/geometry/figures/Rectangle.hpp"
#include "gfx/geometry/Point2d.hpp"
#include "gfx/geometry/Vector2d.hpp"
#include "gfx/geometry/transform/Transformable2d.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
using Trans2d = he::gfx::geometry::transform::Transformable2d<geometry::Point2Df, geometry::Vector2Df>;

class IShape : public Drawable , protected Trans2d
{
protected:
    IShape(const std::string&);
    IShape(const IShape&);

public:
    ~IShape() override;

public:
    void setColor(const he::gfx::Color& color) override;
    const he::gfx::Color getColor() const override;
    bool setPosition(const he::gfx::geometry::Point2Df& position) override;
    const he::gfx::geometry::Point2Df& getPosition() const override;
    const std::string getName() const override;
    void setName(const std::string&) override;
    void setLayerName(const std::string&) override;
    const std::string getLayerName() const override;
    virtual const he::gfx::VertexArray2d& getVertexArray() const;

public:
    bool setOrigin(const he::gfx::geometry::Point2Df& point) override;
    const he::gfx::geometry::Point2Df& getOrigin() const override;
    bool setRotation(const he::gfx::geometry::Angle& angle, const int axis = 0) override;
    const he::gfx::geometry::Angle& getRotation(const int axis = 0) const override;

public:
    virtual void setOriginInCenter() = 0;
    void setOriginPosition(const he::gfx::OriginPosition& originPosition);
    gfx::OriginPosition getOriginPosition() const;

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