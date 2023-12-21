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
    bool setRotation(const float rotationZ) override;
    bool setRotations(const float rotationX, const float rotationY, const float rotationZ, const AxisOrder) override;
    bool setScale(const VECTOR& factors) override;
    bool setPosition(const POINT& position) override;

    const POINT& getOrigin() const override;
    const geometry::Angle& getRotation() const override;
    const RotationArray& getRotations() const override;
    const VECTOR& getScale() const override;
    const POINT& getPosition() const override;

public:
    const Transform& getTransform() const override;
    void transformPoint(POINT& point) override;
    void inverseTransformPoint(geometry::Point2Df& point) override;

private:
    const Transform getRotationMatrix() const;

protected:
    VECTOR m_scale;
    POINT m_origin{};
    POINT m_position{};
    RotationArray m_rotations;

    mutable geometry::transform::Transform m_transform;
    mutable bool m_transformNeedUpdate{true};
};

} // namespace transform
} // namespace geometry
} // namespace gfx
} // namespace he