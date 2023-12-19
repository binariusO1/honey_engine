#pragma once

#include <memory>
#include "gfx/draw/Drawable.hpp"
#include "gfx/geometry/figures/Rectangle.hpp"
#include "gfx/geometry/transform/Transformable.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
template<typename POINT, typename VECTOR, typename VERTEX>
class IShape : public Drawable , protected he::gfx::geometry::transform::Transformable<POINT, VECTOR>
{
protected:
    IShape(const std::string&);
    IShape(const IShape&);

public:
    ~IShape() override;

public:
    void setColor(const Color& color) override;
    bool setPosition(const POINT& position) override;
    void setName(const std::string&) override;
    void setLayerName(const std::string&) override;

    const Color getColor() const override;
    const POINT& getPosition() const override;
    const std::string getName() const override;
    const std::string getLayerName() const override;
    virtual const VERTEX& getVertexArray() const;

public:
    bool setOrigin(const POINT& point) override;
    bool setRotation(const geometry::Angle& angle, const int axis = 0) override;
    const POINT& getOrigin() const override;
    const geometry::Angle& getRotation(const int axis = 0) const override;

public:
    virtual void setOriginInCenter() = 0;
    void setOriginPosition(const OriginPosition& originPosition);
    gfx::OriginPosition getOriginPosition() const;

protected:
    virtual void updateVertexArray() = 0;

protected:
    VERTEX m_vertexArray{};
    Drawable::Context m_context;
    bool m_vertexArrayNeedUpdate{false};

    using TransformableTmpl = geometry::transform::Transformable<POINT, VECTOR>;
};

using IShape2d = IShape<geometry::Point2Df, geometry::Vector2Df, VertexArray2d>;
using IShape2dFor3d = IShape<geometry::Point3Df, geometry::Vector3Df, VertexArray3d>;

} // namespace draw
} // namespace gfx
} // namespace he