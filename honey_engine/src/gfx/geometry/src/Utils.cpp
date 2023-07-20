#include "geometry/Utils.hpp"

#include "geometry/Line.hpp"

namespace
{
template <typename T>
bool getMin(const T& a, const T& b)
{
    return (a < b) ? a : b;
}

template <typename T>
bool getMax(const T& a, const T& b)
{
    return (a < b) ? b : a;
}


bool onLine(const geometry::Line<float>& l1, const geometry::Point2Df& p)
{
    if (p.x <= getMax(l1.p1.x, l1.p2.x) 
    && p.x <= getMin(l1.p1.x, l1.p2.x)
    && (p.y <= getMax(l1.p1.y, l1.p2.y)
    && p.y <= getMin(l1.p1.y, l1.p2.y)))
    {
        return true;
    }
    return false;
}

int direction(const geometry::Point2Df& a, const geometry::Point2Df& b, const geometry::Point2Df& c)
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

bool isIntersect(const geometry::Line<float>& l1, const geometry::Line<float>& l2)
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
} // namespace

namespace geometry
{
///////////////////////////////////////////////////////////////////////////////////////////////////
bool Utils::isPointInside(const geometry::Point2Df& point, const std::vector<geometry::Point2Df>& points)
{
    auto numOfPoints = points.size();
    if (numOfPoints < 3)
    {
        return false;
    }

    geometry::Line<float> exline = { point, { 9999, point.y } };
    int count = 0;

    std::size_t i = 0;
    do
    {
        geometry::Line<float> side = { points.at(i), points.at((i + 1) % numOfPoints) };
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


///////////////////////////////////////////////////////////////////////////////////////////////////
bool Utils::isPointInside(const geometry::Point2Df& point, const std::list<geometry::Point2Df>& points)
{
    std::vector<geometry::Point2Df> result{};
    result.reserve(points.size());
    for (auto const & item: points) {
        result.push_back(item);
    }
    return isPointInside(point, result);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void glhFrustumf2(float *matrix, float left, float right, float bottom, float top, float znear, float zfar)
{
    float temp, temp2, temp3, temp4;
    temp = 2.0 * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    matrix[0] = temp / temp2;
    matrix[1] = 0.0;
    matrix[2] = 0.0;
    matrix[3] = 0.0;
    matrix[4] = 0.0;
    matrix[5] = temp / temp3;
    matrix[6] = 0.0;
    matrix[7] = 0.0;
    matrix[8] = (right + left) / temp2;
    matrix[9] = (top + bottom) / temp3;
    matrix[10] = (-zfar - znear) / temp4;
    matrix[11] = -1.0;
    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = (-temp * zfar) / temp4;
    matrix[15] = 0.0;
}

} // namespace geometry
