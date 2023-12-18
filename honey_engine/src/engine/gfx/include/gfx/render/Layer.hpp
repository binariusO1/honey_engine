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
    bool setPosition(const geometry::Point2Df&) override;

public:
    void addLayer(const std::shared_ptr<he::gfx::render::ILayer>&);
    void addLayers(const LayersList&);
    void removeLayer(const std::shared_ptr<he::gfx::render::ILayer>&);
    
public:
    void addShape(const std::shared_ptr<draw::IShape2d>&);
    void addShapes(const ShapeList&);
    void removeShape(const std::shared_ptr<draw::IShape2d>&);
    he::gfx::render::ShapeList& getShapeList();
    draw::IShape2d& shape(const std::string&);

public:
    void addButton(const std::shared_ptr<gfx::draw::Button>&);
    void addButtons(const ButtonList&);
    he::gfx::render::ButtonList& getButtons();

public:
    void process_event(const he::window::Event&) override;

private:
    void onMouseButtonPressed(const he::window::Event::MouseButtonAction&);
    void onMouseButtonReleased(const he::window::Event::MouseButtonAction&);
    void onMouseCursorMoved(const he::window::Event::MouseMoveEvent&);
    void adjustPointsForEvent(int&, int&);

protected:
    ButtonList m_buttons;
    ShapeList m_shapes; 
    std::set<std::string> m_keys;
    LayersMap m_layers;
    bool m_firstonMouseButtonPressed{false};
};
} // namespace render
} // namespace gfx
} // namespace he