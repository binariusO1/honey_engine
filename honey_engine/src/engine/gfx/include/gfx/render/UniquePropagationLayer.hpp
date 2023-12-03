#pragma once

#include "gfx/render/Layer.hpp"
#include "gfx/render/PropagationSettings.hpp"

namespace he
{
namespace gfx
{
namespace render
{

class UniquePropagationLayer final : public Layer
{
public:
    UniquePropagationLayer(const std::string&, const PropagationSettings&);
    ~UniquePropagationLayer();

public:
    void render(gfx::render::IRender&) override;

public:
    void addButton(const std::shared_ptr<gfx::draw::Button>&);

private:
    void addNewButton(const draw::Button*, const gfx::geometry::Point2Df position, const gfx::geometry::Point2Dt coords);

private:
    PropagationSettings m_propagationSettings{};
};
} // namespace render
} // namespace gfx
} // namespace he