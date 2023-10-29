#pragma once

#include <memory>
#include <string>
#include "exception/invalid_initialization.hpp"
#include "gfx/render/IRenderable.hpp"
#include "gfx/render/VisibilityState.hpp"
#include "window/events/IEventInputListener.hpp"

namespace he
{
namespace gfx
{
namespace render
{
class ILayer;

class ILayer : public IRenderable , public he::window::IEventInputListener
{
public:
    ILayer() = default;
    ~ILayer() = default;

public:
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual bool isVisible() const = 0;

public:
    virtual void render(gfx::render::IRender&) = 0;
    virtual void setRenderSettings(const he::gfx::render::RenderSettings&) = 0;

public:
    virtual const std::string getName() const = 0;

public:
    struct ContextLayer
    {
        ContextLayer(const std::string& name) : name(name)
        {
            if (name.empty())
            {
                throw he::common::invalid_initialization("Name cannot be empty!");
            }
        }
        const std::string name;
        VisibilityState visible{VisibilityState::visible};
    };

};
} // namespace render
} // namespace gfx
} // namespace he