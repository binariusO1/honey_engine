#pragma once

#include "geometry/figures/Figure.hpp"

////////////////////////////////////////
//               3
//               |
//               |   
//        0------2------1
//        :      |
//        :      |
//   (0,0)*- - - 4
///////////////////////////////////////

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
    float m_arm;
private:
    void setSize(const geometry::Size2Df& size) override;


};

} // namespace figures
} // namespace geometry