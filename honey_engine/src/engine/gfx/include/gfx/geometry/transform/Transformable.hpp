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
template<typename POINT, typename VECTOR>
class Transformable : public ITransformable<POINT, VECTOR>
{
public:
    Transformable();
    Transformable(const Transformable&) = default;
    ~Transformable() override;

public:
    void move(const VECTOR& offset) override;
    bool setOrigin(const POINT& origin) override;
    bool setRotation(const geometry::Angle& angle, const int axis = 0) override;
    bool setScale(const VECTOR& factors) override;
    bool setPosition(const POINT& position) override;

    const POINT& getOrigin() const override;
    const geometry::Angle& getRotation(const int axis = 0) const override;
    const VECTOR& getScale() const override;
    const POINT& getPosition() const override;

public:
    const Transform& getTransform() const override;
    void transformPoint(POINT& point) override;
    void inverseTransformPoint(POINT& point) override;

protected:
    VECTOR m_scale; //todo : initialization 1.0f, 1.0f
    POINT m_origin{}; // 0.0f, 0.0f};
    POINT m_position{}; // 0.0f, 0.0f};
    geometry::Angle m_rotation[3]{he::gfx::geometry::Angle(0.0), he::gfx::geometry::Angle(0.0), he::gfx::geometry::Angle(0.0)};
    //int[3] m_rotation{0,0,0};

    mutable geometry::transform::Transform m_transform;
    mutable bool m_transformNeedUpdate{true};
};

} // namespace transform
} // namespace geometry
} // namespace gfx
} // namespace he