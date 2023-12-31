#pragma once

#include "exception/invalid_initialization.hpp"
#include "gfx/Color.hpp"
#include "gfx/Vertex.hpp"
#include "gfx/draw/OriginPosition.hpp"
#include "gfx/render/VisibilityState.hpp"
#include "gfx/draw/IDrawable.hpp"

namespace he
{
namespace gfx
{
namespace geometry::figures
{
class Figure;
} // namespace geometry::figure
namespace draw
{

class Drawable : public IDrawable
{
public:
    virtual ~Drawable() override = default;

public:
    virtual void setColor(const he::gfx::Color&) = 0;
    virtual const he::gfx::Color getColor() const = 0;
    virtual void setLayerName(const std::string&) = 0;
    virtual const std::string getLayerName() const = 0;

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
        // TODO: add const hash id
        std::string name;
        std::string layerName;
        he::gfx::OriginPosition originPosition{he::gfx::OriginPosition::leftDown};
        he::gfx::Color color{he::gfx::Color::White};
        VisibilityState visibility{VisibilityState::visible};
    };
};
} // namespace draw
} // namespace gfx
} // namespace he