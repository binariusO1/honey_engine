#pragma once

#include "geometry/figures/Figure.hpp"

////////////////////////////////////////
//          5________4
//          /        \*
//        0/          \ 3
//         \          /
//          \        /
//   (0,0)*  1      2
///////////////////////////////////////
namespace geometry
{
namespace figures
{
class Hexagon : public Figure
{
public:
    Hexagon(const float side);

public:
    std::size_t getNumOfPoints() const;
    geometry::Point2Df getCenterPoint() const;
    geometry::Point2Df getPoint(const std::size_t index) const;
    bool isPointInside(const geometry::Point2Df& point) const;

    void setSize(const geometry::Size2Df& size) {};
    const geometry::Size2Df& getSize() const { return m_size; };

public:
    void setSize(const float side);
    float getSideLenght() const;

private:
    float m_side;
    geometry::Size2Df m_size{};
};

} // namespace figures
} // namespace geometry