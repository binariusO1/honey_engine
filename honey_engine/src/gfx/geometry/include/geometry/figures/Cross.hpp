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
    std::size_t getNumOfPoints() const;
    geometry::Point2Df getCenterPoint() const;
    geometry::Point2Df getPoint(const std::size_t index) const;
    bool isPointInside(const geometry::Point2Df& point) const;

    void setSize(const geometry::Size2Df& size) {};
    const geometry::Size2Df& getSize() const { return m_size; };

public:
    void setSize(const float arm);
    float getArmLenght() const;

private:
    float m_arm;
    geometry::Size2Df m_size{};
};

} // namespace figures
} // namespace geometry