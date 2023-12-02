#include "RenderFixture.hpp"
#include <gtest/gtest.h>
#include "gfx/draw/Button.hpp"
#include "gfx/render/CopyPropagationLayer.hpp"
#include "gfx/render/PropagationSettings.hpp"

using namespace ::testing;
namespace
{
constexpr he::gfx::geometry::Size2Dpxl t_buttonSize(200, 100);
} // namespace

namespace  he
{
namespace gfx
{
class PropagationLayerSCT : public testing::Test , public RenderFixture
{
public:
    PropagationLayerSCT()
    {
    }
    ~PropagationLayerSCT() = default;

    void createCustomScreen()
    {
        PropagationSettings propagationSettings;
        mainPropagationLayer = std::make_shared<he::gfx::render::CopyPropagationLayer>("main_propagation_layer", propagationSettings);
        mainScene = std::make_shared<he::gfx::render::Scene>("main_scene");
        mainScene->addLayer(mainPropagationLayer);
        he::gfx::render::SceneTransitionTable transitionTable{{mainScene, nullptr, nullptr, nullptr, nullptr}};
        mainSceneManager = std::make_unique<he::gfx::render::SceneManager>(transitionTable);
    }

    std::shared_ptr<he::gfx::render::CopyPropagationLayer> mainPropagationLayer;
};

TEST_F(PropagationLayerSCT, menuTest_afterAddButton_shouldPropagateWithDifferentCallbacks)
{
    createCustomScreen();
    he::gfx::draw::Button button1("Button1", t_buttonSize);
    button1.setColor(he::gfx::Color::Blue);
    button1.setText("Click");
;
    // mainPropagationLayer->addButton(std::make_shared<he::gfx::draw::Button>(button1));

    display(500);
}
}// namespace gfx
}// namespace he