#pragma once

#include <set>
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
    he::gfx::render::DrawableList& drawableList();

public:
    void addButton(const std::shared_ptr<gfx::draw::IButton>&);

public:
    void process_event(const he::window::Event&) override;

private:
    void onMauseButtonPressed(const he::window::Event::MouseButtonAction&);

protected:
    DrawableList m_uniqueDrawables; 
    std::set<std::string> m_uniqueKeys;
    ButtonList m_buttons;
    LayersMap m_layers;
    bool m_firstOnMauseButtonPressed{false};
};
} // namespace render
} // namespace gfx
} // namespace he