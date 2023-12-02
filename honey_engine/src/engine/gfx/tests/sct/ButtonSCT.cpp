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

    he::gfx::draw::Button button1("Button1", t_buttonSize);
    button1.setColor(he::gfx::Color::Blue);
    button1.setText("Click");
    button1.setPosition({574, 350});
    std::shared_ptr<he::gfx::draw::Button> but1 = std::make_shared<he::gfx::draw::Button>(button1);

    addDrawableToMainLayer(but1);
    addButtonToMainLayer(but1);

    std::shared_ptr<he::window::IEventInputListener> sceneListener = std::shared_ptr<he::gfx::render::SceneManager>(mainSceneManager);
    mainWindow->addEventInputListener(sceneListener.get());

    display(300);

    but1->setPosition({231, 150});
    display(500);
}

TEST_F(ButtonSCT, eventTest_afterMoveButtonWithDefinedTextToWindowCenter_shouldGetMouseButtonEvent)
{
    createCustomScreen();

    he::gfx::draw::Button button1("Button1", t_buttonSize);
    button1.setColor(he::gfx::Color::Red);
    button1.setPosition({f_defaultWindowWidth/2.f, f_defaultWindowHeight/2.f});
    button1.setOriginInCenter();
    he::gfx::draw::Text customText("customText");
    customText.setFont(f_fontCalibriPath);
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

TEST_F(ButtonSCT, eventTest_afterGetMouseButtonEvent_shouldRunCallbackAndChangeText)
{
    createCustomScreen();

    he::gfx::draw::Button button1("Button1", t_buttonSize);
    button1.setColor(he::gfx::Color::Magenta);
    button1.setText("Click");
    button1.setPosition({f_defaultWindowWidth/2.f, f_defaultWindowHeight/2.f});
    button1.setOriginInCenter();
    std::shared_ptr<he::gfx::draw::Button> but1 = std::make_shared<he::gfx::draw::Button>(button1);
    bool textChanged{false};
    he::gfx::draw::ButtonCallback callback = [&but1, &textChanged](){
        if (textChanged)
        {
            but1->setText("Click");
        }
        else
        {
            but1->setText("Just changed");
        }
        textChanged = not textChanged;
    };
    but1->addCallback(callback);

    addDrawableToMainLayer(but1);
    addButtonToMainLayer(but1);

    std::shared_ptr<he::window::IEventInputListener> sceneListener = std::shared_ptr<he::gfx::render::SceneManager>(mainSceneManager);
    mainWindow->addEventInputListener(sceneListener.get());

    display(500);
}

TEST_F(ButtonSCT, eventTest_afterGetMouseButtonEvent_shouldRunCallbackAndChangeTextCharacterSize)
{
    createCustomScreen();

    he::gfx::draw::Button button1("Button1", t_buttonSize);
    button1.setColor(he::gfx::Color::Magenta);
    button1.setPosition({f_defaultWindowWidth/2.f, f_defaultWindowHeight/2.f});
    button1.setOriginInCenter();
    he::gfx::draw::Text customText("customText");
    customText.setFont(f_fontCalibriPath);
    unsigned int characterSize{93};
    customText.setCharacterSize(characterSize);
    customText.setColor(gfx::Color::Yellow);
    customText.setOriginInCenter();
    customText.setString("Click");
    button1.setText(customText);
    std::shared_ptr<he::gfx::draw::Button> but1 = std::make_shared<he::gfx::draw::Button>(button1);

    he::gfx::draw::ButtonCallback callback = [&but1, &customText, &characterSize](){
        customText.setCharacterSize(++characterSize);
            but1->setText(customText);
    };
    but1->addCallback(callback);

    addDrawableToMainLayer(but1);
    addButtonToMainLayer(but1);

    std::shared_ptr<he::window::IEventInputListener> sceneListener = std::shared_ptr<he::gfx::render::SceneManager>(mainSceneManager);
    mainWindow->addEventInputListener(sceneListener.get());

    display(500);
}
}// namespace gfx
}// namespace he