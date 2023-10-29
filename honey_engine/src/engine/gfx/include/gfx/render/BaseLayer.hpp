#pragma once

#include <map>
#include <memory>
#include <string>
#include "gfx/draw/IDrawable.hpp"
#include "gfx/render/ILayer.hpp"


namespace he
{
namespace gfx
{
namespace render
{
using DrawableList = std::vector<std::shared_ptr<he::gfx::draw::IDrawable>>;
using DrawableMap = std::map<std::string, std::shared_ptr<he::gfx::draw::IDrawable>>;
using EventInputListenerList = std::vector<std::shared_ptr<he::window::IEventInputListener>>;
using EventInputListenerMap = std::map<std::string, std::shared_ptr<he::window::IEventInputListener>>;
using LayersList = std::vector<std::shared_ptr<he::gfx::render::ILayer>>;
using LayersMap = std::map<std::string, std::shared_ptr<he::gfx::render::ILayer>>;

class BaseLayer : public ILayer
{
public:
    BaseLayer(const std::string&);
    ~BaseLayer() override;

public:
    void show() override;
    void hide() override;
    bool isVisible() const override;

public:
    const std::string getName() const override;

public:
    virtual void render(gfx::render::IRender&) = 0;
    virtual void setRenderSettings(const he::gfx::render::RenderSettings&) = 0;

public:
    virtual void process_event(const he::window::Event&) = 0;

protected:
    ILayer::ContextLayer m_context;
    he::gfx::render::RenderSettings m_renderSettings{};
};
} // namespace render
} // namespace gfx
} // namespace he