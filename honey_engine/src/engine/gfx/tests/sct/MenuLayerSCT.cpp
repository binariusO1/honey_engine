#include "RenderFixture.hpp"
#include <gtest/gtest.h>
#include "gfx/render/MenuLayer.hpp"
#include "gfx/render/PropagationSettings.hpp"

using namespace ::testing;
namespace
{
constexpr he::gfx::geometry::Size2Dpxl t_buttonSize(160, 60);
} // namespace

namespace  he
{
namespace gfx
{
class MenuLayerSCT : public testing::Test , public RenderFixture
{
public:
    MenuLayerSCT()
    {
    }
    ~MenuLayerSCT() = default;

    void createCustomScreen(const PropagationSettings& propagationSettings)
    {
        mainMenuLayer = std::make_shared<he::gfx::render::MenuLayer>("menu_layer", propagationSettings);
        mainScene = std::make_shared<he::gfx::render::Scene>("main_scene");
        mainScene->addLayer(mainMenuLayer);
        he::gfx::render::SceneTransitionTable transitionTable{{mainScene, nullptr, nullptr, nullptr, nullptr}};
        mainSceneManager = std::make_unique<he::gfx::render::SceneManager>(transitionTable);

        enableEventInputListener();
    }

    std::shared_ptr<he::gfx::render::MenuLayer> mainMenuLayer;
};

TEST_F(MenuLayerSCT, menuTest_afterAddButtonSetMenuOriginToCenterAndSetPosition_shouldCreateMenuInWindowCenter)
{
    PropagationSettings propagationSettings{1, 5, 0, -30};
    createCustomScreen(propagationSettings);

    he::gfx::draw::Button button1("Button1", t_buttonSize);
    button1.setColor(he::gfx::Color(217, 210, 205, 255));
    button1.setPosition({0, 600});
    he::gfx::draw::Text customText("customText");
    customText.setFont(f_fontCalibriPath);
    customText.setCharacterSize(28);
    customText.setColor(gfx::Color::Black);
    button1.setText(customText);
    mainMenuLayer->setButton(std::make_shared<he::gfx::draw::Button>(button1));
    
    auto buttons = mainMenuLayer->getButtons();
    buttons[0]->setText("Continue");
    buttons[1]->setText("New Game");
    buttons[2]->setText("Options");
    buttons[3]->setText("Credits");
    buttons[4]->setText("Exit");

    buttons[0]->setCallback(
        [button = buttons[0]](bool){button->setColor(gfx::Color::Green);}, 
        window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}));
    buttons[1]->setCallback(
        [button = buttons[1]](bool){button->setColor(gfx::Color::Red);}, 
        window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}));
    buttons[2]->setCallback(
        [button = buttons[2]](bool){button->setColor(gfx::Color::Blue);}, 
        window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}));
    buttons[3]->setCallback(
        [button = buttons[3]](bool){button->setColor(gfx::Color::Yellow);}, 
        window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}));
    buttons[4]->setCallback(
        [button = buttons[4]](bool){button->setColor(gfx::Color::Magenta);}, 
        window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}));
    
    mainMenuLayer->setOrigin(gfx::OriginPosition::center);
    mainMenuLayer->setPosition({f_defaultWindowWidth/2.f, f_defaultWindowHeight/2.f});

    display(500);
}

TEST_F(MenuLayerSCT, menuTest_afterAddButtonAndSetMenuPosition_shouldCreateHorizontalMenu)
{
    PropagationSettings propagationSettings{5, 1, 1, 0};
    createCustomScreen(propagationSettings);

    he::gfx::draw::Button button1("Button1", {60, 60});
    button1.setColor(he::gfx::Color(217, 210, 205, 255));
    button1.setPosition({0, 740});
    he::gfx::draw::Text customText("customText");
    customText.setFont(f_fontCalibriPath);
    customText.setCharacterSize(28);
    customText.setColor(gfx::Color::Black);
    button1.setText(customText);
    mainMenuLayer->setButton(std::make_shared<he::gfx::draw::Button>(button1));

    auto buttons = mainMenuLayer->getButtons();
    buttons[0]->setText("1");
    buttons[1]->setText("2");
    buttons[2]->setText("3");
    buttons[3]->setText("4");
    buttons[4]->setText("5");

    display(500);
}
}// namespace gfx
}// namespace he