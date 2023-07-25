#pragma once

#include "geometry/Point2d.hpp"
#include "geometry/Size2d.hpp"

namespace geometry
{
namespace figures
{
class Figure
{
public:
    Figure() = default;
    Figure(Figure&) = default;
    virtual ~Figure() = default;

public:
    virtual std::size_t getNumOfPoints() const = 0;
    virtual geometry::Point2Df getCenterPoint() const = 0;
    virtual geometry::Point2Df getPoint(const std::size_t) const = 0;
    virtual const geometry::Size2Df getSize() const = 0;

    virtual void setSize(const geometry::Size2Df&) = 0;

    virtual bool isPointInside(const geometry::Point2Df&) const = 0;
};
} // namespace figures
} // namespace geometry