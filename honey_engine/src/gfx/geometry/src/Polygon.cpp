#include "geometry/figures/Polygon.hpp"

#include "geometry/Line.hpp"
#include "geometry/Utils.hpp"

namespace geometry
{
namespace figures
{
////////////////////////////////////////////////////////////
std::size_t Polygon::getNumOfPoints() const
{
    return m_points.size();
}


////////////////////////////////////////////////////////////
geometry::Point2Df Polygon::getCenterPoint() const
{
    // LOG_WARNING << "Polygon::getCenterPoint() not implemented yet";
    return geometry::Point2Df{};
}


////////////////////////////////////////////////////////////
geometry::Point2Df Polygon::getPoint(const std::size_t index) const
{
    if (index <= m_points.size())
    {
        auto it = m_points.begin();
        std::advance(it, index);
        return *it;
    }
    throw std::out_of_range("Given index is not correctly");
    return geometry::Point2Df();
}


////////////////////////////////////////////////////////////
bool Polygon::isPointInside(const geometry::Point2Df& point) const
{
    return geometry::Utils::isPointInside(point, m_points);
}


////////////////////////////////////////////////////////////
void Polygon::addPoint(const geometry::Point2Df point)
{
    m_points.push_back(point);
}


////////////////////////////////////////////////////////////
void Polygon::removePoint(const std::size_t index)
{
    if (index <= m_points.size())
    {
        auto it = m_points.begin();
        std::advance(it, index);
        m_points.pop_back();
    }
    throw std::out_of_range("Given index is not correctly");
}


////////////////////////////////////////////////////////////
std::list<geometry::Point2Df> Polygon::getPoints() const
{
    return m_points;
}
} // namespace figures
} // namespace geometry