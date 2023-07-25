#pragma once

#include "exception/invalid_initialization.hpp"
#include "gfx/Color.hpp"
#include "gfx/Vertex.hpp"
#include "gfx/draw/OriginPosition.hpp"
#include "gfx/geometry/Point2d.hpp"
#include "gfx/render/RenderSettings.hpp"
#include "gfx/render/VisibilityState.hpp"

namespace he
{
namespace gfx
{
namespace render
{
class Render;
} // namespace render
namespace draw
{
class IDrawable
{
public:
    virtual ~IDrawable() = default;

public:
    virtual void setColor(const he::gfx::Color&) = 0;
    virtual void setPosition(const he::gfx::geometry::Point2Df&) = 0;
    virtual he::gfx::Color getColor() const = 0;
    virtual const std::string getName() const = 0;
    virtual const he::gfx::geometry::Point2Df& getPosition() const = 0;

    virtual unsigned int getTextureId() const = 0;

public:
    virtual const he::gfx::VertexArray2d& getVertexArray() const = 0;
    virtual void setClosedVertexArray(bool) = 0;

protected:
    friend class he::gfx::render::Render;
    virtual void draw(he::gfx::render::Render&, he::gfx::render::RenderSettings&) const = 0;

public:
    struct Context
    {
        Context(const std::string& name) : name(name)
        {
            if (name.empty())
            {
                throw he::common::invalid_initialization("Name cannot be empty!");
            }
        }
        const std::string name;
        he::gfx::OriginPosition originPosition{he::gfx::OriginPosition::leftDown};
        he::gfx::Color color{he::gfx::Color::White};
        VisibilityState visibility{VisibilityState::visible};
    };
};
} // namespace draw
} // namespace gfx
} // namespace he