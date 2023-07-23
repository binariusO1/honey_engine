#pragma once

#include "Color.hpp"
#include "geometry/Point3d.hpp"
#include "geometry/Point2d.hpp"

namespace gfx
{
class Vertex3d
{
public:
    Vertex3d() = default;
    Vertex3d(const geometry::Point3Df& pos) : position(pos) {}
    Vertex3d(const geometry::Point3Df& pos, const Color& col) : position(pos) , color(col) {} 
    Vertex3d(const geometry::Point3Df& pos, const geometry::Point2Df& tCoords) : position(pos) , texCoords(tCoords) {} 
    Vertex3d(const geometry::Point3Df& pos, const Color& col, const geometry::Point2Df& tCoords) : position(pos) , color(col) , texCoords(tCoords) {} 

public:
    geometry::Point3Df      position;                   //!< 3D position of the vertex
    Color                   color{Color::White};    //!< Color of the vertex
    geometry::Point2Df      texCoords;                  //!< Coordinates of the texture's pixel to map to the vertex
};

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

template<class T>
class VertexTexture
{
public:
    VertexTexture() = default;
    VertexTexture(const T& pos) : position(pos) {}
    VertexTexture(const T& pos, const T& tCoords) : position(pos) , texCoords(tCoords) {} 

public:
    T      position;
    T      texCoords;
};

template<class T>
class VertexPrimitive
{
public:
    VertexPrimitive() = default;
    VertexPrimitive(const T& pos) : position(pos) {}

public:
    T              position;
};

using Vertex2d          = Vertex<geometry::Point2Df>;
using Vertex2dTexture   = VertexTexture<geometry::Point2Df>;
using Vertex2dPrimitive = VertexPrimitive<geometry::Point2Df>;

} // namespace gfx