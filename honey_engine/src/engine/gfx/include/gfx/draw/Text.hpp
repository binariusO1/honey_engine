#pragma once

#include <memory>
#include "gfx/Vertex.hpp"
#include "gfx/draw/IDrawable.hpp"
#include "gfx/geometry/transform/Transformable2d.hpp"

namespace he
{
namespace gfx
{
namespace text
{
class Font;
} // namespace text
namespace draw
{
class Text : public IDrawable , protected he::gfx::geometry::transform::Transformable2d
{
public:
    Text(const std::string&);
    Text(const Text& copy) = default;
    Text(Text&& copy) = default;
    ~Text() override;

public:
    void setString(const std::string& string);
    const std::string& getString() const;
    void setFont(const text::Font& font);

public:
    void setColor(const he::gfx::Color&) override {};
    void setOrigin(const he::gfx::geometry::Point2Df&) override {};
    void setOriginInCenter() override {};
    void setPosition(const he::gfx::geometry::Point2Df&) override {};
    void setRotation(const he::gfx::geometry::Angle&) override {};
    he::gfx::Color getColor() const override { return m_context.color; };
    const std::string getName() const override { return m_context.name; };
    const he::gfx::geometry::Point2Df& getOrigin() const override { return Transformable2d::getOrigin(); };
    const he::gfx::geometry::Point2Df& getPosition() const override { return Transformable2d::getPosition(); };
    const he::gfx::geometry::Angle& getRotation() const override { return Transformable2d::getRotation(); };

    unsigned int getTextureId() const override { return 0; };

private:
    const he::gfx::VertexArray2d& getVertexArray() const override { return m_vertexArray; };
    void draw(const he::gfx::render::Render&, const he::gfx::render::RenderSettings&) const override {};

private:
    he::gfx::VertexArray2d m_vertexArray{};
    IDrawable::Context m_context;
    std::string m_string; 
};
} // namespace draw
} // namespace gfx
} // namespace he