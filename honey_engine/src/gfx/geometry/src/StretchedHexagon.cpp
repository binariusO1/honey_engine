#include "geometry/figures/StretchedHexagon.hpp"

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
StretchedHexagon::StretchedHexagon()
{
    // LOG_MEMORY << LOG_CONSTRUCTOR;
}


////////////////////////////////////////////////////////////
StretchedHexagon::~StretchedHexagon()
{
    // LOG_MEMORY << LOG_DESTRUCTOR;
}


////////////////////////////////////////////////////////////
StretchedHexagon::StretchedHexagon(const geometry::Size2Df& size)
{
    // LOG_MEMORY << LOG_CONSTRUCTOR;
    setSize({size.width, size.height});
}


////////////////////////////////////////////////////////////
StretchedHexagon::StretchedHexagon(const StretchedHexagon& copy)
{
    // LOG_MEMORY << LOG_COPY_CONSTRUCTOR;
    m_size.height = copy.m_size.height;
    m_size.width = copy.m_size.width;
}


////////////////////////////////////////////////////////////
void StretchedHexagon::setSize(const geometry::Size2Df& size)
{
    m_size.width = size.width;
    m_size.height = size.height;
}


////////////////////////////////////////////////////////////
const geometry::Size2Df StretchedHexagon::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
std::size_t StretchedHexagon::getNumOfPoints() const
{
    return 6;
}


////////////////////////////////////////////////////////////
geometry::Point2Df StretchedHexagon::getCenterPoint() const
{
    return geometry::Point2Df( m_size.width/2.f , m_size.height/2.f);
}


////////////////////////////////////////////////////////////
geometry::Point2Df StretchedHexagon::getPoint(const std::size_t index) const
{
    switch (index)
    {
        case 0:
            return geometry::Point2Df( 0.0f , m_size.height * 0.5f);
        case 1:
            return geometry::Point2Df( m_size.height * 0.25f , 0.0f );
        case 2:
            return geometry::Point2Df( m_size.width - m_size.height * 0.25f, 0.0f );
        case 3:
            return geometry::Point2Df( m_size.width, m_size.height * 0.5f);
        case 4:
            return geometry::Point2Df( m_size.width - m_size.height * 0.25f, m_size.height );
        case 5:
            return geometry::Point2Df( m_size.height * 0.25f , m_size.height );
        default:
            throw std::out_of_range("Given index is not correctly");
            return geometry::Point2Df();
    }
}


////////////////////////////////////////////////////////////
bool StretchedHexagon::isPointInside(const geometry::Point2Df& point) const
{

    float a = 0.25f * MATH_SQRT3;
    float dy = getAbsoluteValue(point.y - getCenterPoint().y) / (m_size.height*2.f/MATH_SQRT3);

    // hexagon on the left side
    float dx = getAbsoluteValue(point.x - 0.5f*m_size.height) / (m_size.height*2.f/MATH_SQRT3);
    bool status_1 = (dy <= a) && (a * dx + 0.25f * dy <= 0.5f * a);

    // hexagon on the right side
    dx = getAbsoluteValue(point.x - m_size.width + 0.5f*m_size.height) / (m_size.height*2.f/MATH_SQRT3);
    bool status_2 = (dy <= a) && (a * dx + 0.25f * dy <= 0.5f * a);

    // rectangle
    bool status_3 = (point.x >= 0.25f*m_size.height and point.x <= m_size.width - 0.25f*m_size.height) and (point.y >= 0.0f and point.y <= m_size.height);
    return status_1 or status_2 or status_3;
}
} // namespace figures
} // namespace geometry