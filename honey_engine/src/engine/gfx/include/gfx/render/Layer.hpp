#pragma once

#include "gfx/draw/IDrawable.hpp"
#include "gfx/render/BaseLayer.hpp"
#include "gfx/render/RenderSettings.hpp"

namespace he
{
namespace gfx
{
namespace render
{

class Layer : public BaseLayer
{
public:
    Layer(const std::string&);
    ~Layer();

public:
    void render(gfx::render::IRender&) override;
    void setRenderSettings(const he::gfx::render::RenderSettings&) override;

public:
    void addLayer(const std::shared_ptr<he::gfx::render::ILayer>&);
    void addLayers(const LayersList&);
    void removeLayer(const std::shared_ptr<he::gfx::render::ILayer>&);
    
public:
    void addDrawable(const std::shared_ptr<he::gfx::draw::IDrawable>&);
    void addDrawables(const DrawableList&);
    // TODO add funtion which is add shape, text as template and convert to idrawable (after checking)
    void removeDrawable(const std::shared_ptr<he::gfx::draw::IDrawable>&);
    he::gfx::draw::IDrawable& drawable(const std::string&);

public:
    void process_event(const he::window::Event&) override;

protected:
    DrawableList m_uniqueDrawables;
    EventInputListenerMap m_uniqueListeners;
    LayersMap m_layers;
};
} // namespace render
} // namespace gfx
} // namespace he