#pragma once

#include <memory>
#include "gfx/draw/Drawable.hpp"
#include "gfx/geometry/figures/Rectangle.hpp"
#include "gfx/geometry/Point2d.hpp"
#include "gfx/geometry/Vector2d.hpp"
#include "gfx/geometry/transform/Transformable.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
template<typename POINT, typename VECTOR, typename VERTEX>
class IShape : public Drawable , protected  he::gfx::geometry::transform::Transformable<POINT, VECTOR>
{
using TransformableTmpl = he::gfx::geometry::transform::Transformable<POINT, VECTOR>;

protected:
    IShape(const std::string&);
    IShape(const IShape&);

public:
    ~IShape() override;

public:
    void setColor(const he::gfx::Color& color) override;
    bool setPosition(const POINT& position) override;
    void setName(const std::string&) override;
    void setLayerName(const std::string&) override;

    const he::gfx::Color getColor() const override;
    const POINT& getPosition() const override;
    const std::string getName() const override;
    const std::string getLayerName() const override;
    virtual const VERTEX& getVertexArray() const;

public:
    bool setOrigin(const POINT& point) override;
    bool setRotation(const he::gfx::geometry::Angle& angle, const int axis = 0) override;
    const POINT& getOrigin() const override;
    const he::gfx::geometry::Angle& getRotation(const int axis = 0) const override;

public:
    virtual void setOriginInCenter() = 0;
    void setOriginPosition(const he::gfx::OriginPosition& originPosition);
    gfx::OriginPosition getOriginPosition() const;

protected:
    virtual void updateVertexArray() = 0;

protected:
    VERTEX m_vertexArray{};
    Drawable::Context m_context;
    bool m_vertexArrayNeedUpdate{false};
};
} // namespace draw
} // namespace gfx
} // namespace he