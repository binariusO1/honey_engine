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
using ShapeList = std::vector<std::shared_ptr<he::gfx::draw::IShape2d>>;

template<typename POINT, typename VECTOR, typename VERTEX>
class Layer : public BaseLayer<POINT, VECTOR>
{
private:
    using BaseLayerTmpl = BaseLayer<POINT, VECTOR>;
    using IShapeTmpl = draw::IShape<POINT, VECTOR, VERTEX>;
    using IShapeListTmpl = std::vector<std::shared_ptr<draw::IShape<POINT, VECTOR, VERTEX>>>;

public:
    Layer(const std::string&);
    ~Layer();

public:
    void render(gfx::render::IRender&) override;
    void setRenderSettings(const he::gfx::render::RenderSettings&) override;

public:
    bool setPosition(const POINT&) override;

public:
    void addLayer(const std::shared_ptr<he::gfx::render::ILayer>&);
    void addLayers(const LayersList&);
    void removeLayer(const std::shared_ptr<he::gfx::render::ILayer>&);
    
public:
    void addShape(const std::shared_ptr<IShapeTmpl>&);
    void addShapes(const IShapeListTmpl&);
    void removeShape(const std::shared_ptr<IShapeTmpl>&);
    IShapeListTmpl& getShapeList();
    IShapeTmpl& shape(const std::string&);

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
    IShapeListTmpl m_shapes; 
    std::set<std::string> m_keys;
    LayersMap m_layers;
    bool m_firstonMouseButtonPressed{false};
};

using Layer2d = Layer<geometry::Point2Df, geometry::Vector2Df, VertexArray2d>;
using Layer3d = Layer<geometry::Point3Df, geometry::Vector3Df, VertexArray3d>;

} // namespace render
} // namespace gfx
} // namespace he