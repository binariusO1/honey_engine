#pragma once

#include <vector>
#include <list>
#include "geometry/Point2d.hpp"

namespace geometry
{
class Utils
{
public:
    static bool isPointInside(const geometry::Point2Df& point, const std::vector<geometry::Point2Df>& points);
    static bool isPointInside(const geometry::Point2Df& point, const std::list<geometry::Point2Df>& points);

    static void perspective(float *matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar);
};

} // namespace geometry