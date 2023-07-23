#include "VertexArray2d.hpp"

#include "logger/Logger.hpp"
namespace gfx
{
////////////////////////////////////////////////////////////
VertexArray2d::VertexArray2d() = default;


////////////////////////////////////////////////////////////
std::size_t VertexArray2d::size() const
{
    return m_vertices.size();
}


////////////////////////////////////////////////////////////
void VertexArray2d::fillByDefault(const std::size_t numOfElements)
{
    clear();
    m_vertices.resize(numOfElements);
    std::fill(m_vertices.begin(), m_vertices.end(), gfx::Vertex2d( geometry::Point2Df{0, 0}, geometry::Point2Df{0, 0} ));
}


////////////////////////////////////////////////////////////
bool VertexArray2d::empty() const
{
    return m_vertices.empty();
}


////////////////////////////////////////////////////////////
Vertex2d& VertexArray2d::operator[](std::size_t index)
{
    return m_vertices[index];
}


////////////////////////////////////////////////////////////
Vertex2d VertexArray2d::operator[](std::size_t index) const
{
    return m_vertices[index];
}


////////////////////////////////////////////////////////////
void VertexArray2d::push_back(const Vertex2d& vertex)
{
    m_vertices.push_back(vertex);
}


////////////////////////////////////////////////////////////
void VertexArray2d::clear()
{
    m_vertices.clear();
}


////////////////////////////////////////////////////////////
void VertexArray2d::draw(gfx::render::Render& target, const gfx::render::RenderSettings& settings)
{
    if (not m_vertices.empty())
    {
        target.draw(m_vertices.data(), m_vertices.size(), settings);
    }
}
} // namespace gfx
