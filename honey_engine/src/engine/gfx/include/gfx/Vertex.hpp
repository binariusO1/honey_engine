#pragma once

#include <vector>
#include "gfx/Color.hpp"
#include "gfx/geometry/Point2d.hpp"

namespace he
{
namespace gfx
{

template<class T>
class Vertex
{
public:
    Vertex() = default;
    Vertex(const T& pos) : position(pos) {}
    Vertex(const T& pos, const Color& col) : position(pos) , color(col) {} 
    Vertex(const T& pos, const T& tCoords) : position(pos) , texCoords(tCoords) {} 
    Vertex(const T& pos, const Color& col, const T& tCoords) : position(pos) , color(col) , texCoords(tCoords) {} 

public:
    T                       position;
    Color                   color{Color::White};
    T                       texCoords;
};

using Vertex2d = Vertex<he::gfx::geometry::Point2Df>;
using VertexArray2d = std::vector<he::gfx::Vertex2d>;

} // namespace gfx
} // namespace he