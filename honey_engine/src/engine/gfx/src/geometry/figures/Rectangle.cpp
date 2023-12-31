#include "gfx/geometry/figures/Rectangle.hpp"

namespace he
{
namespace gfx
{
namespace geometry
{
namespace figures
{
////////////////////////////////////////////////////////////
Rectangle::Rectangle()
{
}


////////////////////////////////////////////////////////////
Rectangle::~Rectangle()
{
}


////////////////////////////////////////////////////////////
Rectangle::Rectangle(const float side)
{
    setSize({side, side});
}


////////////////////////////////////////////////////////////
Rectangle::Rectangle(const geometry::Size2Df& size)
{
    setSize(size);
}


////////////////////////////////////////////////////////////
Rectangle::Rectangle(const Rectangle& copy)
{
    m_size = copy.m_size;
}


////////////////////////////////////////////////////////////
 std::size_t Rectangle::getNumOfPoints() const
{
    return 4;
}


////////////////////////////////////////////////////////////
geometry::Point2Df Rectangle::getCenterPoint() const
{
    return geometry::Point2Df{static_cast<float>(m_size.width) * 0.5f , static_cast<float>(m_size.height) * 0.5f};
}


////////////////////////////////////////////////////////////
geometry::Point2Df Rectangle::getPoint(const std::size_t index) const
{
    switch (index)
    {
        case 0:
            return geometry::Point2Df(0.0f, 0.0f);
        case 1:
            return geometry::Point2Df(static_cast<float>(m_size.width), 0.0f);
        case 2:
            return geometry::Point2Df(static_cast<float>(m_size.width), static_cast<float>(m_size.height));
        case 3:
            return geometry::Point2Df(0.0f, static_cast<float>(m_size.height));
        default:
            throw std::out_of_range("Given index is not correctly");
    }
}


////////////////////////////////////////////////////////////
bool Rectangle::isPointInside(const geometry::Point2Df& point) const
{
    return (point.x >= 0.0f and point.x <= m_size.width) and (point.y >= 0.0f and point.y <= m_size.height);

}


////////////////////////////////////////////////////////////
void Rectangle::setSize(const geometry::Size2Df& size)
{
    m_size = size;
}


////////////////////////////////////////////////////////////
const geometry::Size2Df Rectangle::getSize() const
{
    return m_size;
}


} // namespace figures
} // namespace geometry
} // namespace gfx
} // namespace he