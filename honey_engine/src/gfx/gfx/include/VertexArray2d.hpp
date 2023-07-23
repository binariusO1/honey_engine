#pragma once

#include <vector>
#include "Vertex.hpp"
// #include "draw2d/Drawable.hpp"

namespace gfx
{
class VertexArray2d : public gfx::render::Drawable
{
public:
    VertexArray2d();

public:
    std::size_t size() const;
    void push_back(const Vertex2d&);
    bool empty() const;
    void fillByDefault(const std::size_t);
    void clear();

public:
    Vertex2d& operator[](std::size_t);
    Vertex2d operator[](std::size_t) const;

protected:
    friend class gfx::render::Render;
    void draw(gfx::render::Render&, const gfx::render::RenderSettings&) override; 

private:
    std::vector<Vertex2d>       m_vertices;                             //!< Vertices contained in the array
    // PrimitiveType           m_primitiveType{PrimitiveType::Points}; //!< Type of primitives to draw
};
} // namespace gfx