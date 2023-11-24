#pragma once

#include "gfx/geometry/transform/ITransformable.hpp"

namespace he
{
namespace gfx
{
namespace geometry
{
namespace transform
{

class Transformable2d : public ITransformable
{
public:
    Transformable2d() = default;
    Transformable2d(const Transformable2d&) = default;
    ~Transformable2d() override;

public:
    void move(const geometry::Vector2Df& offset) override;
    void setOrigin(const Point2Df& origin) override;
    void setRotation(const geometry::Angle& angle) override;
    void setScale(const geometry::Vector2Df& factors) override;
    void setPosition(const Point2Df& position) override;

    const Point2Df& getOrigin() const override;
    const geometry::Angle& getRotation() const override;
    const Vector2Df& getScale() const override;
    const Point2Df& getPosition() const override;

public:
    const Transform& getTransform() const override;
    void transformPoint(Point2Df& point) override;
    void inverseTransformPoint(Point2Df& point) override;

protected:
    geometry::Vector2Df     m_scale{1.0f, 1.0f};
    geometry::Point2Df      m_origin{0.0f, 0.0f};
    geometry::Point2Df      m_position{0.0f, 0.0f};
    geometry::Angle         m_rotation{he::gfx::geometry::Angle(0.0)};

    mutable geometry::transform::Transform m_transform;
    mutable bool m_transformNeedUpdate{true};
};
} // namespace transform
} // namespace geometry
} // namespace gfx
} // namespace he