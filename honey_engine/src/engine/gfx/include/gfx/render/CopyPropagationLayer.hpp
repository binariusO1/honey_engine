#pragma once

#include <map>
#include <string>
#include "gfx/draw/Shape.hpp"
#include "gfx/render/BaseLayer.hpp"
#include "gfx/render/PropagationSettings.hpp"
#include "gfx/render/RenderSettings.hpp"

namespace he
{
namespace gfx
{
namespace render
{

class CopyPropagationLayer final : public BaseLayer
{
public:
    CopyPropagationLayer(const std::string&, const PropagationSettings&);
    ~CopyPropagationLayer();

public:
    void render(gfx::render::IRender&) override;
    void setRenderSettings(const he::gfx::render::RenderSettings&) override;
    
public:
    void addShape(const std::shared_ptr<he::gfx::draw::IShape>&);
    void addDrawables(const ShapeList&);
    void removeDrawable(const std::shared_ptr<he::gfx::draw::Shape>&);
    he::gfx::draw::IShape& drawable(const std::string&);

public:
    void process_event(const he::window::Event&) override;

private:
    ShapeList m_uniqueShapes{};
    PropagationSettings m_propagationSettings{};
};
} // namespace render
} // namespace gfx
} // namespace he