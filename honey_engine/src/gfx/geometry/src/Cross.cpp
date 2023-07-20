#include "geometry/figures/Cross.hpp"

namespace geometry
{
namespace figures
{
////////////////////////////////////////////////////////////
Cross::Cross(const float arm)
{
    setSize(arm);
}


////////////////////////////////////////////////////////////
void Cross::setSize(const float arm)
{
    m_arm = arm;
}


////////////////////////////////////////////////////////////
float Cross::getArmLenght() const
{
    return m_arm;
}


////////////////////////////////////////////////////////////
std::size_t Cross::getNumOfPoints() const
{
    return 5;
}


////////////////////////////////////////////////////////////
geometry::Point2Df Cross::getCenterPoint() const
{
    return geometry::Point2Df(m_arm, m_arm);
}


////////////////////////////////////////////////////////////
geometry::Point2Df Cross::getPoint(const std::size_t index) const
{
    switch (index)
    {
        case 0:
            return geometry::Point2Df( 0.0f , m_arm );
        case 1:
            return geometry::Point2Df( m_arm * 2.0f, m_arm );
        case 2:
            return geometry::Point2Df( m_arm , m_arm );
        case 3:
            return geometry::Point2Df( m_arm , m_arm * 2.0f );
        case 4:
            return geometry::Point2Df( m_arm, 0.0f );
        default:
            throw std::out_of_range("Given index is not correctly");
            return geometry::Point2Df();
    }
}


////////////////////////////////////////////////////////////
bool Cross::isPointInside(const geometry::Point2Df& point) const
{
    return (point.x == m_arm and (point.y >= 0.0f or point.y <= m_arm * 2.0f)) or (point.y == m_arm and (point.x >= 0 or point.x <= m_arm * 2.0f));
}
} // namespace figures
} // namespace geometry