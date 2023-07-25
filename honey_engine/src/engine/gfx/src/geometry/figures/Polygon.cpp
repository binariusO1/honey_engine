#include "gfx/geometry/figures/Polygon.hpp"

#include <vector>
#include "gfx/geometry/Line.hpp"
#include "gfx/geometry/Utils.hpp"
#include "math/Functions.hpp"


namespace
{
bool onLine(const he::gfx::geometry::Line<float>& l1, const he::gfx::geometry::Point2Df& p)
{
    if (p.x <= math::getMax(l1.p1.x, l1.p2.x) 
    && p.x <= math::getMin(l1.p1.x, l1.p2.x)
    && p.y <= math::getMax(l1.p1.y, l1.p2.y)
    && p.y <= math::getMin(l1.p1.y, l1.p2.y))
    {
        return true;
    }
    return false;
}

int direction(const he::gfx::geometry::Point2Df& a, const he::gfx::geometry::Point2Df& b, const he::gfx::geometry::Point2Df& c)
{
    int val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
 
    if (val == 0)
    {
        return 0;
    }
    else if (val < 0)
    {
        return 2;
    }
    return 1;
}

bool isIntersect(const he::gfx::geometry::Line<float>& l1, const he::gfx::geometry::Line<float>& l2)
{
    int dir1 = direction(l1.p1, l1.p2, l2.p1);
    int dir2 = direction(l1.p1, l1.p2, l2.p2);
    int dir3 = direction(l2.p1, l2.p2, l1.p1);
    int dir4 = direction(l2.p1, l2.p2, l1.p2);
 
    if (dir1 != dir2 && dir3 != dir4)
    {
        return true;
    }

    if (dir1 == 0 && onLine(l1, l2.p1))
    {
        return true;
    }

    if (dir2 == 0 && onLine(l1, l2.p2))
    {
        return true;
    }

    if (dir3 == 0 && onLine(l2, l1.p1))
    {
        return true;
    }

    if (dir4 == 0 && onLine(l2, l1.p2))
    {
        return true;
    }

    return false;
}

bool isPointInside(const he::gfx::geometry::Point2Df& point, const std::vector<he::gfx::geometry::Point2Df>& points)
{
    auto numOfPoints = points.size();
    if (numOfPoints < 3)
    {
        return false;
    }

    he::gfx::geometry::Line<float> exline = { point, { 9999, point.y } };
    int count = 0;

    std::size_t i = 0;
    do
    {
        he::gfx::geometry::Line<float> side = { points.at(i), points.at((i + 1) % numOfPoints) };
        if (isIntersect(side, exline)) 
        {
                if (direction(side.p1, point, side.p2) == 0)
                {
                    return onLine(side, point);
                }
                count++;
        }
        i = (i + 1) % numOfPoints;
    } while (i != 0);
    return count & 1;
}

float getPointXCenterOfPolygon(const float area, const std::list<he::gfx::geometry::Point2Df>& points)
{
    float pointX = 0.0;
    float xi = 0.0;
    float yi = 0.0;
    float fxi = 0.0;
    float fyi = 0.0;
    bool isFirst{true};
    for (const auto &point : points)
    {
        if (isFirst)
        {
            isFirst = false;
            xi = point.x;
            fxi = xi;
            yi = point.y;
            fyi = yi;
            continue;
        }
        else
        {
            pointX += (( xi + point.x ) * ( xi * point.y - point.x * yi ));
            xi = point.x;
            yi = point.y;
        }
    }
    pointX += (( xi + fxi ) * ( xi * fyi - fxi * yi ));
    pointX /= (6.0 * area);
    return pointX;
}

float getPointYCenterOfPolygon(const float area, const std::list<he::gfx::geometry::Point2Df>& points)
{
    float pointY = 0.0;
    float xi = 0.0;
    float yi = 0.0;
    float fxi = 0.0;
    float fyi = 0.0;
    bool isFirst{true};
    for (const auto &point : points)
    {
        if (isFirst)
        {
            isFirst = false;
            xi = point.x;
            fxi = xi;
            yi = point.y;
            fyi = yi;
            continue;
        }
        else
        {
            pointY += (( yi + point.y ) * ( xi * point.y - point.x * yi ));
            xi = point.x;
            yi = point.y;
        }
    }
    pointY += (( yi + fyi ) * ( xi * fyi - fxi * yi ));
    pointY /= (6.0 * area);
    return pointY;
}

he::gfx::geometry::Point2Df getCenterOfPolygon(const float area, const std::list<he::gfx::geometry::Point2Df>& points)
{
    return he::gfx::geometry::Point2Df{getPointXCenterOfPolygon(area, points), getPointYCenterOfPolygon(area, points)};
}

float getPolygonsSignedArea(const std::list<he::gfx::geometry::Point2Df>& points)
{
    float area = 0.0;
    float fxi = 0.0;
    float fyi = 0.0;
    float xi = 0.0;
    float yi = 0.0;
    bool isFirst{true};
    for (const auto &point : points)
    {
        if (isFirst)
        {
            isFirst = false;
            xi = point.x;
            fxi = xi;
            yi = point.y;
            fyi = yi;
            continue;
        }
        else
        {
            area += (( xi * point.y ) - ( point.x * yi ));
            xi = point.x;
            yi = point.y;
        }
    }
    area += (( xi * fyi ) - ( fxi * yi ));
    area *= 0.5f;
    return area;
}
} // namespace

namespace he
{
namespace gfx
{
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

    geometry::Point2Df centerPoint{0.0, 0.0};

    // note: Centroid of a set of points
    if (m_points.size() < 3)
    {
        for (const auto point : m_points)
        {
            centerPoint.x += point.x;
            centerPoint.y += point.y;
        }
        centerPoint.x = centerPoint.x / m_points.size();
        centerPoint.y = centerPoint.y / m_points.size();
    }
    // note: Centroid of a set of vertexes for polygon
    else
    {
        centerPoint = getCenterOfPolygon(::getPolygonsSignedArea(m_points), m_points);
    }
    return centerPoint;
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
}


////////////////////////////////////////////////////////////
bool Polygon::isPointInside(const geometry::Point2Df& point) const
{
    std::vector<geometry::Point2Df> result{};
    result.reserve(m_points.size());
    for (auto const & item: m_points)
    {
        result.push_back(item);
    }
    return ::isPointInside(point, result);
}


////////////////////////////////////////////////////////////
void Polygon::setPoints(const std::list<geometry::Point2Df>& points)
{
    m_points = points;
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
    else
    {
        throw std::out_of_range("Given index is not correctly");
    }
}


////////////////////////////////////////////////////////////
std::list<geometry::Point2Df> Polygon::getPoints() const
{
    return m_points;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
const geometry::Size2Df Polygon::getSize() const
{
    throw std::logic_error("Function not inherited");
}


////////////////////////////////////////////////////////////
void Polygon::setSize(const geometry::Size2Df& size)
{
    throw std::logic_error("Function not inherited");
}


} // namespace figures
} // namespace geometry
} // namespace gfx
} // namespace he