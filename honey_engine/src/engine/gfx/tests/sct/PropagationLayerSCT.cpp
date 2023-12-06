#include "RenderFixture.hpp"
#include <gtest/gtest.h>
#include "gfx/draw/Button.hpp"
#include "gfx/render/UniquePropagationLayer.hpp"
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

    void createCustomScreen(const PropagationSettings& propagationSettings)
    {
        mainPropagationLayer = std::make_shared<he::gfx::render::UniquePropagationLayer>("main_propagation_layer", propagationSettings);
        mainScene = std::make_shared<he::gfx::render::Scene>("main_scene");
        mainScene->addLayer(mainPropagationLayer);
        he::gfx::render::SceneTransitionTable transitionTable{{mainScene, nullptr, nullptr, nullptr, nullptr}};
        mainSceneManager = std::make_unique<he::gfx::render::SceneManager>(transitionTable);
    }

    std::shared_ptr<he::gfx::render::UniquePropagationLayer> mainPropagationLayer;
};

TEST_F(PropagationLayerSCT, menuTest_afterAddButton_shouldPropagateWithDifferentCallbacks)
{
    PropagationSettings propagationSettings{3, 3, 0, 0};
    createCustomScreen(propagationSettings);
    enableEventInputListener();
    he::gfx::draw::Button button1("Button1", t_buttonSize);
    button1.setColor(he::gfx::Color::Blue);
    button1.setText("Click once");

    mainPropagationLayer->addButton(std::make_shared<he::gfx::draw::Button>(button1));
    auto buttons = mainPropagationLayer->getButtons();

    for (std::size_t i = 0 ; i < buttons.size() ; ++i)
    {
        const std::string iterator = std::to_string(i+1);
        const he::gfx::draw::ButtonCallback callback = [button = buttons[i], iterator](){
            button->setText("button_" + iterator);
            button->removeCallback(window::Event(window::Event::mouseButtonPressed, window::Event::MouseMoveEvent{window::Mouse::Button::Left}));
        };
        buttons[i]->setCallback(callback, window::Event(window::Event::mouseButtonPressed, window::Event::MouseMoveEvent{window::Mouse::Button::Left}));
    }

    display(500);
}
}// namespace gfx
}// namespace he