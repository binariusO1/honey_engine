#pragma once

#include "gfx/geometry/figures/Figure.hpp"

////////////////////////////////////////
//          5________4
//          /        \*
//        0/          \ 3
//         \          /
//          \        /
//   (0,0)*  1      2
////////////////////////////////////////

namespace he
{
namespace gfx
{
namespace geometry
{
namespace figures
{
class Hexagon : public Figure
{
public:
    Hexagon(const float side);

public:
    std::size_t getNumOfPoints() const override;
    geometry::Point2Df getCenterPoint() const override;
    geometry::Point2Df getPoint(const std::size_t index) const override;
    const geometry::Size2Df getSize() const override;

    bool isPointInside(const geometry::Point2Df& point) const override;

public:
    float getSideLenght() const;

    void setSide(const float side);

private:
    void setSize(const geometry::Size2Df& size) override;

    float m_side;
};

} // namespace figures
} // namespace geometry
} // namespace gfx
} // namespace he