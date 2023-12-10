#pragma once

#include "gfx/render/Layer.hpp"
#include "gfx/render/PropagationSettings.hpp"

namespace he
{
namespace gfx
{
namespace render
{

class UniquePropagationLayer : public Layer
{
public:
    UniquePropagationLayer(const std::string&, const PropagationSettings&);
    ~UniquePropagationLayer();

public:
    void render(gfx::render::IRender&) override;

public:
    virtual void addButton(const std::shared_ptr<gfx::draw::Button>&);
    void update();

protected:
    void checkAndCorrectPropagationParameters();
    void updateButtons(const std::size_t startIndex);

private:
    float calcPos(const float buttonDimension, const float firstButtonPosCoord, const float propSettingsDistance, std::size_t iterator);
    float calcXPos(const draw::Button*, const std::size_t x);
    float calcYPos(const draw::Button*, const std::size_t y);

protected:
    PropagationSettings m_propagationSettings{};
};
} // namespace render
} // namespace gfx
} // namespace he