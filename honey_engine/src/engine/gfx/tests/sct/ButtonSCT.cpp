#include "RenderFixture.hpp"
#include <gtest/gtest.h>
#include "gfx/draw/Button.hpp"
#include "gfx/draw/AdvButton.hpp"
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

    void createCustomScreen()
    {
        RenderFixture::createCustomScreen();
        enableEventInputListener();
    }

    draw::Button createCustomButtonInWindowCenter()
    {
        he::gfx::draw::Button button1("Button1", t_buttonSize);
        button1.setColor(he::gfx::Color::Magenta);
        button1.setPosition({f_defaultWindowWidth/2.f, f_defaultWindowHeight/2.f});
        button1.setOriginInCenter();
        return button1;
    }

    draw::AdvButton createCustomAdvButtonInWindowCenter()
    {
        he::gfx::draw::AdvButton button1("Button1", t_buttonSize);
        button1.setColor(he::gfx::Color::Blue);
        button1.setPosition({f_defaultWindowWidth/2.f, f_defaultWindowHeight/2.f});
        button1.setOriginInCenter();
        return button1;
    }
};

TEST_F(ButtonSCT, eventTest_afterMoveButtonWithDefaultTextToCustomPosition_shouldNotGetUndefinedMouseButtonEvent)
{
    createCustomScreen();

    he::gfx::draw::Button button1("Button1", t_buttonSize);
    button1.setColor(he::gfx::Color::Blue);
    button1.setText("No callback");
    button1.setPosition({574, 350});
    std::shared_ptr<he::gfx::draw::Button> but1 = std::make_shared<he::gfx::draw::Button>(button1);

    addButtonToMainLayer(but1);

    display(150);

    but1->setPosition({231, 150});
    display(150);
}

TEST_F(ButtonSCT, eventTest_afterMoveButtonWithDefinedTextToWindowCenter_shouldNotGetUndefinedMouseButtonEvent)
{
    createCustomScreen();

    he::gfx::draw::Button button1("Button1", t_buttonSize);
    button1.setColor(he::gfx::Color::Red);
    button1.setPosition({f_defaultWindowWidth/2.f, f_defaultWindowHeight/2.f});
    button1.setOriginInCenter();
    he::gfx::draw::Text customText("customText");
    customText.setFont(f_fontCalibriPath);
    customText.setCharacterSize(30);
    customText.setColor(gfx::Color::Yellow);
    customText.setOriginInCenter();
    customText.setString("No callback");
    button1.setText(customText);
    std::shared_ptr<he::gfx::draw::Button> but1 = std::make_shared<he::gfx::draw::Button>(button1);

    addButtonToMainLayer(but1);

    display(250);
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
    he::gfx::draw::ButtonCheckCallback callback = [&but1, &textChanged](bool){
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
    but1->setCallback(callback, window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}));

    addButtonToMainLayer(but1);

    display(450);
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

    he::gfx::draw::ButtonCheckCallback callback = [&but1, &customText, &characterSize](bool){
        customText.setCharacterSize(++characterSize);
            but1->setText(customText);
    };

    but1->setCallback(callback, window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}));

    addButtonToMainLayer(but1);

    display(500);
}

TEST_F(ButtonSCT, eventTest_afterGetMouseButtonEvent_shouldRunProperCallbackAndChangeText)
{
    createCustomScreen();

    std::shared_ptr<he::gfx::draw::Button> button = std::make_shared<he::gfx::draw::Button>(createCustomButtonInWindowCenter());
    button->setText("Click");

    he::gfx::draw::ButtonCheckCallback callbackLeft = [&button](bool){
        button->setText("Left button");
        button->removeCallback(window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left})); // TODO - zmienić event na predefiniowany enum żeby fraza była krótsza
    };

    he::gfx::draw::ButtonCheckCallback callbackRight = [&button](bool){
        button->setText("Right button");
        button->removeCallback(window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Right}));
    };

    button->setCallback(callbackLeft, window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}));
    button->setCallback(callbackRight, window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Right}));
    addButtonToMainLayer(button);

    display(500);
}

TEST_F(ButtonSCT, eventTest_afterGetMouseMoveEventInside_shouldRunCallbackAndChangeColor)
{
    createCustomScreen();

    std::shared_ptr<he::gfx::draw::Button> button = std::make_shared<he::gfx::draw::Button>(createCustomButtonInWindowCenter());

    button->setText("Touch");
    he::gfx::draw::ButtonCheckCallback callback = [&button](bool isPointInside){
        if (isPointInside)
        {
            button->setColor(gfx::Color::Green);
            button->setText("Touched");
            button->removeCallback(window::Event(window::Event::mouseCursorMoved));
        }
    };

    button->setCallback(callback, window::Event(window::Event::mouseCursorMoved));
    addButtonToMainLayer(button);
    display(500);
}

TEST_F(ButtonSCT, eventTest_afterGetMouseMoveEventInsideAndOutside_shouldRunCallbackAndChangeColor)
{
    createCustomScreen();

    std::shared_ptr<he::gfx::draw::Button> button = std::make_shared<he::gfx::draw::Button>(createCustomButtonInWindowCenter());

    button->setText("Touch");
    bool isTouched{false};
    he::gfx::draw::ButtonCheckCallback callback = [&button, &isTouched](bool isPointInside){
        if (isPointInside and not isTouched)
        {
            button->setColor(gfx::Color::Green);
            button->setText("Touched");
            isTouched = true;
        }
        else if(not isPointInside and isTouched)
        {
            button->setColor(gfx::Color::Red);
            button->setText("Touch");
            isTouched = false;
        }
    };

    button->setCallback(callback, window::Event(window::Event::mouseCursorMoved));
    addButtonToMainLayer(button);
    main2dLayer->setPosition({200, 100});
    display(500);
}

TEST_F(ButtonSCT, eventTest_afterMousePressedAndReleased_shouldRunCallbackAndChangeColor)
{
    createCustomScreen();

    std::shared_ptr<he::gfx::draw::Button> button = std::make_shared<he::gfx::draw::Button>(createCustomButtonInWindowCenter());

    button->setColor(gfx::Color::Red);
    button->setText("Hold");

    he::gfx::draw::ButtonCheckCallback callbackPressed = [&button](bool isTouched){
        button->setColor(gfx::Color::Green);
        button->setText("Release");
    };
    he::gfx::draw::ButtonCheckCallback callbackReleased = [&button](bool){
        button->setColor(gfx::Color::Red);
        button->setText("Hold");
    };

    button->setCallback(callbackPressed, window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}));
    button->setCallback(callbackReleased, window::Event(window::Event::mouseButtonReleased, window::Event::MouseButtonAction{window::Mouse::Button::Left}));
    addButtonToMainLayer(button);

    display(500);
}

TEST_F(ButtonSCT, stateTest_afterSetStateCallbacks_shouldRunCallbacks)
{
    createCustomScreen();

    std::shared_ptr<he::gfx::draw::AdvButton> button = std::make_shared<he::gfx::draw::AdvButton>(createCustomAdvButtonInWindowCenter());

    he::gfx::draw::ButtonCallback callbackIdleTouched = [&button](){
        button->setColor(gfx::Color::Cyan);
    };
    he::gfx::draw::ButtonCallback callbackTouchedIdle = [&button](){
        button->setColor(gfx::Color::Blue);
    };
    he::gfx::draw::ButtonCallback callbackTouchedClicked = [&button](){
        button->setColor(gfx::Color::Green);
    };
    he::gfx::draw::ButtonCallback callbackClickedTouched = [&button](){
        button->setColor(gfx::Color::Cyan);
    };
    he::gfx::draw::ButtonCallback callbackClickedIdle = [&button](){
        button->setColor(gfx::Color::Blue);
    };

    button->setCallback(callbackIdleTouched, {ButtonState::Idle, ButtonState::Touched});
    button->setCallback(callbackTouchedIdle, {ButtonState::Touched, ButtonState::Idle});
    button->setCallback(callbackTouchedClicked, {ButtonState::Touched, ButtonState::Clicked});
    button->setCallback(callbackClickedTouched, {ButtonState::Clicked, ButtonState::Touched});
    button->setCallback(callbackClickedIdle, {ButtonState::Clicked, ButtonState::Idle});

    addButtonToMainLayer(button);
    display(500);
}
}// namespace gfx
}// namespace he