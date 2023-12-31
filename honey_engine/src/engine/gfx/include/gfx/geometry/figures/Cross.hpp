#pragma once

#include "gfx/geometry/figures/Figure.hpp"

////////////////////////////////////////
//               3
//               |
//               |   
//        0------2------1
//        :      |
//        :      |
//   (0,0)*- - - 4
///////////////////////////////////////

namespace he
{
namespace gfx
{
namespace geometry
{
namespace figures
{
class Cross : public Figure
{
public:
    Cross(const float arm);

public:
    std::size_t getNumOfPoints() const override;
    geometry::Point2Df getCenterPoint() const override;
    geometry::Point2Df getPoint(const std::size_t index) const override;
    const geometry::Size2Df getSize() const override;

    bool isPointInside(const geometry::Point2Df& point) const override;

public:
    float getArmLenght() const;

    void setSize(const float arm);

private:
    void setSize(const geometry::Size2Df& size) override;
    float m_arm;

};

} // namespace figures
} // namespace geometry
} // namespace gfx
} // namespace he