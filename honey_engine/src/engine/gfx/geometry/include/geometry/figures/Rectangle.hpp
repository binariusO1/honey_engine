#pragma once

#include "geometry/figures/Figure.hpp"

////////////////////////////////////////
//          3--------2
//          |        |
//          |        |
//          0--------1
//   (0,0)*  
///////////////////////////////////////

namespace geometry
{
namespace figures
{
class Rectangle : public Figure
{
public:    
    Rectangle();
    ~Rectangle();
    Rectangle(const float side);
    Rectangle(const geometry::Size2Df& size);
    Rectangle(const Rectangle& copy);

public:
    std::size_t getNumOfPoints() const override;
    geometry::Point2Df getCenterPoint() const override;
    geometry::Point2Df getPoint(const std::size_t index) const override;
    bool isPointInside(const geometry::Point2Df&) const override;

public:
    void setSize(const geometry::Size2Df& size);
    const geometry::Size2Df getSize() const;

private:
    geometry::Size2Df m_size;


friend bool operator ==(const Rectangle& left, const Rectangle& right)
{
    return (left.m_size.width == right.m_size.width) and (left.m_size.height == right.m_size.height);
}

friend bool operator !=(const Rectangle& left, const Rectangle& right)
{
    return (left.m_size.width != right.m_size.width) or (left.m_size.height != right.m_size.height);
}
};

} // namespace figures
} // namespace geometry