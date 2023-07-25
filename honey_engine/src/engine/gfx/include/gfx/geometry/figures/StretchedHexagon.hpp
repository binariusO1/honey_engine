#pragma once

#include "gfx/geometry/figures/Figure.hpp"

////////////////////////////////////////
//          5_______l_____________4
//          /|         |           \*
//        0/_|         |          __\ 3
//         \ |h        |h           /
//          \|         |           /
//   (0,0)*  1                    2
///////////////////////////////////////

namespace he
{
namespace gfx
{
namespace geometry
{
namespace figures
{
class StretchedHexagon : public Figure
{
public:
    StretchedHexagon();
    ~StretchedHexagon();
    StretchedHexagon(const geometry::Size2Df& size);
    StretchedHexagon(const StretchedHexagon& copy);

public:
    std::size_t getNumOfPoints() const override;
    geometry::Point2Df getCenterPoint() const override;
    geometry::Point2Df getPoint(const std::size_t index) const override;

    bool isPointInside(const geometry::Point2Df& point) const override;

public:
    void setSize(const geometry::Size2Df& size) override;
    const geometry::Size2Df getSize() const override;

private:
    geometry::Size2Df m_size;


friend bool operator ==(const StretchedHexagon& left, const StretchedHexagon& right)
{
    return (left.m_size.height == right.m_size.height) and (left.m_size.width == right.m_size.width);
}

friend bool operator !=(const StretchedHexagon& left, const StretchedHexagon& right)
{
    return (left.m_size.height != right.m_size.height) or (left.m_size.width != right.m_size.width);
}
};

} // namespace figures
} // namespace geometry
} // namespace gfx
} // namespace he