#include "RenderFixture.hpp"
#include <gtest/gtest.h>
#include "gfx/draw/Button.hpp"
#include "gfx/graphic/Texture.hpp"

using namespace ::testing;
namespace
{
constexpr he::gfx::geometry::Size2Dpxl t_buttonSize(200, 100);
} // namespace


namespace  he
{
namespace gfx
{
class ButtonSCT : public testing::Test , public RenderFixture
{
public:
    ButtonSCT()
    {
    }
    ~ButtonSCT() = default;
};

TEST_F(ButtonSCT, eventTest_afterMoveButtonWithDefaultTextToCustomPosition_shouldGetMouseButtonEvent)
{
    createCustomScreen();

    std::shared_ptr<he::gfx::render::Texture> texture1 = std::make_shared<he::gfx::render::Texture>();
    texture1->createEmpty(t_buttonSize);
    he::gfx::draw::Button button1("Button1", texture1);
    button1.setColor(he::gfx::Color::Blue);
    button1.setText("Quit");
    button1.setPosition({574, 350});
    std::shared_ptr<he::gfx::draw::Button> but1 = std::make_shared<he::gfx::draw::Button>(button1);

    addDrawableToMainLayer(but1);
    addButtonToMainLayer(but1);

    std::shared_ptr<he::window::IEventInputListener> sceneListener = std::shared_ptr<he::gfx::render::SceneManager>(mainSceneManager);
    mainWindow->addEventInputListener(sceneListener.get());

    display(75);

    but1->setPosition({231, 150});
    display(500);
}

TEST_F(ButtonSCT, eventTest_afterMoveButtonWithDefinedTextToWindowCenter_shouldGetMouseButtonEvent)
{
    createCustomScreen();

    std::shared_ptr<he::gfx::render::Texture> texture1 = std::make_shared<he::gfx::render::Texture>();
    texture1->createEmpty(t_buttonSize);
    he::gfx::draw::Button button1("Button1", texture1);
    button1.setColor(he::gfx::Color::Blue);
    button1.setPosition({defaultWindowWidth/2.f, defaultWindowHeight/2.f});
    button1.setOriginInCenter();
    he::gfx::draw::Text customText("customText");
    customText.setFont(t_fontCalibriPath);
    customText.setCharacterSize(40);
    customText.setColor(gfx::Color::Yellow);
    customText.setOriginInCenter();
    customText.setString("Click");
    button1.setText(customText);
    std::shared_ptr<he::gfx::draw::Button> but1 = std::make_shared<he::gfx::draw::Button>(button1);

    addDrawableToMainLayer(but1);
    addButtonToMainLayer(but1);

    std::shared_ptr<he::window::IEventInputListener> sceneListener = std::shared_ptr<he::gfx::render::SceneManager>(mainSceneManager);
    mainWindow->addEventInputListener(sceneListener.get());

    display(500);
}
}// namespace gfx
}// namespace he