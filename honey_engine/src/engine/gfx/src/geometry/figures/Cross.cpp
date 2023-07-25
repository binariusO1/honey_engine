#include "gfx/geometry/figures/Cross.hpp"

namespace he
{
namespace gfx
{
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
    }
}


////////////////////////////////////////////////////////////
const geometry::Size2Df Cross::getSize() const
{
    return geometry::Size2Df{m_arm * 2.0f, m_arm * 2.0f};
}


////////////////////////////////////////////////////////////
bool Cross::isPointInside(const geometry::Point2Df& point) const
{
    return (point.x == m_arm and (point.y >= 0.0f or point.y <= m_arm * 2.0f)) or (point.y == m_arm and (point.x >= 0 or point.x <= m_arm * 2.0f));
}


////////////////////////////////////////////////////////////
float Cross::getArmLenght() const
{
    return m_arm;
}


////////////////////////////////////////////////////////////
void Cross::setSize(const float arm)
{
    m_arm = arm;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
void Cross::setSize(const geometry::Size2Df& size)
{
    throw std::logic_error("Function not inherited");
}


} // namespace figures
} // namespace geometry
} // namespace gfx
} // namespace he