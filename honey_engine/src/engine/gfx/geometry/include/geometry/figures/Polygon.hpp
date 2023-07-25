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
    std::size_t getNumOfPoints() const override;
    geometry::Point2Df getCenterPoint() const override;
    geometry::Point2Df getPoint(const std::size_t index) const override;

    bool isPointInside(const geometry::Point2Df& point) const override;

public:
    void setPoints(const std::list<geometry::Point2Df>& points);
    void addPoint(const geometry::Point2Df point);
    void removePoint(const std::size_t index);
    std::list<geometry::Point2Df> getPoints() const;

private:
    void setSize(const geometry::Size2Df& size) override;
    const geometry::Size2Df getSize() const override;

    std::list<geometry::Point2Df> m_points{};
};

} // namespace figures
} // namespace geometry