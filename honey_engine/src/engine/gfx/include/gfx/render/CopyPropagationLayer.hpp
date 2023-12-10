#pragma once

#include "gfx/render/Layer.hpp"
#include "gfx/render/PropagationSettings.hpp"

namespace he
{
namespace gfx
{
namespace render
{

class CopyPropagationLayer : public Layer
{
public:
    CopyPropagationLayer(const std::string&, const PropagationSettings&);
    ~CopyPropagationLayer();

public:
    void render(gfx::render::IRender&) override;

private:
    void process_event(const he::window::Event&) override {};
    void addButton(const std::shared_ptr<gfx::draw::Button>&) {};

    PropagationSettings m_propagationSettings{};
};
} // namespace render
} // namespace gfx
} // namespace he