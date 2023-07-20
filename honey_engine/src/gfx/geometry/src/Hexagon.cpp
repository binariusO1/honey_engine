#include "geometry/figures/Hexagon.hpp"

#include "math/Constants.hpp"

namespace
{
template<typename T>
T getAbsoluteValue(const T& val)
{
    return ((val < 0) ? (val * (-1)) : val);
}
} // namespace
namespace geometry
{
namespace figures
{
////////////////////////////////////////////////////////////
Hexagon::Hexagon(const float side)
{
    setSize(side);
}


////////////////////////////////////////////////////////////
void Hexagon::setSize(const float side)
{
    m_side = side;
}


////////////////////////////////////////////////////////////
float Hexagon::getSideLenght() const
{
    return m_side;
}


////////////////////////////////////////////////////////////
std::size_t Hexagon::getNumOfPoints() const
{
    return 6;
}


////////////////////////////////////////////////////////////
geometry::Point2Df Hexagon::getCenterPoint() const
{
    return geometry::Point2Df( m_side , ((m_side * MATH_SQRT3) * 0.5f) );
}


////////////////////////////////////////////////////////////
geometry::Point2Df Hexagon::getPoint(const std::size_t index) const
{
    switch (index)
    {
        case 0:
            return geometry::Point2Df( 0.0f , ((m_side * MATH_SQRT3) * 0.5f) );
        case 1:
            return geometry::Point2Df( m_side * 0.5f , 0.0f );
        case 2:
            return geometry::Point2Df( m_side * 1.5f , 0.0f );
        case 3:
            return geometry::Point2Df( m_side * 2.0f , ((m_side * MATH_SQRT3) * 0.5f) );
        case 4:
            return geometry::Point2Df( m_side * 1.5f , (m_side * MATH_SQRT3) );
        case 5:
            return geometry::Point2Df( m_side * 0.5f , (m_side * MATH_SQRT3) );
        default:
            throw std::out_of_range("Given index is not correctly");
            return geometry::Point2Df();
    }
}


////////////////////////////////////////////////////////////
bool Hexagon::isPointInside(const geometry::Point2Df& point) const
{
    float dx = getAbsoluteValue(point.x - getCenterPoint().x) / (m_side * 2.0f);
    float dy = getAbsoluteValue(point.y - getCenterPoint().y) / (m_side * 2.0f);
    float a = 0.25f * MATH_SQRT3;
    bool status = (dy <= a) && (a * dx + 0.25f * dy <= 0.5f * a);
    
    return status;
}
} // namespace figures
} // namespace geometry