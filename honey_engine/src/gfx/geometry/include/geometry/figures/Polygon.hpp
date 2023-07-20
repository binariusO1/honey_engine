#pragma once

#include <list>
#include "geometry/figures/Figure.hpp"

namespace geometry
{
namespace figures
{
class Polygon : public Figure
{
public:
    Polygon() = default;

public:
    std::size_t getNumOfPoints() const;
    geometry::Point2Df getCenterPoint() const;
    geometry::Point2Df getPoint(const std::size_t index) const;
    bool isPointInside(const geometry::Point2Df& point) const;

    void setSize(const geometry::Size2Df& size) {};
    const geometry::Size2Df getSize() const { return m_size; };

public:
    void addPoint(const geometry::Point2Df point);
    void removePoint(const std::size_t index);
    std::list<geometry::Point2Df> getPoints() const;

protected:
    std::list<geometry::Point2Df> m_points{};
    geometry::Size2Df m_size{};
};

} // namespace figures
} // namespace geometry