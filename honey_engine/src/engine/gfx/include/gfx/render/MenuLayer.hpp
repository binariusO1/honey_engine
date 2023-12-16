#pragma once

#include "gfx/render/UniquePropagationLayer.hpp"

namespace he
{
namespace gfx
{
namespace render
{
class MenuLayer : public UniquePropagationLayer
{
public:
    MenuLayer(const std::string&, const PropagationSettings&);
    ~MenuLayer() = default;

public:
    void render(gfx::render::IRender&) override;

public:
    bool setPosition(const geometry::Point2Df&) override;
    void setOrigin(gfx::OriginPosition);

public:
    void setButton(const std::shared_ptr<gfx::draw::Button>&);

private:
    void addButton(const std::shared_ptr<gfx::draw::Button>&) override;
    void setPositionToCenter(const geometry::Point2Df& position);

private:
    bool isButtonSet{false};
    gfx::OriginPosition m_originPosition{gfx::OriginPosition::leftDown};
};
} // namespace render
} // namespace gfx
} // namespace he