#include <gtest/gtest.h>
#include "TextFixture.hpp"
#include "draw/TextMock.hpp"
#include "gfx/draw/Button.hpp"
#include "graphic/TextureMock.hpp"
#include "text/FontMock.hpp"
#include "window/events/Event.hpp"

using namespace ::testing;

namespace
{
constexpr he::gfx::geometry::Size2Dpxl t_buttonSize{100, 100};
} // namespace

namespace he::gfx::draw
{
class ButtonTestsFixture : public text::TextFixture
{
public:
    void createSut()
    {
        sut = std::make_unique<he::gfx::draw::Button>("button1", textureMock);
    }

    void expectInitCalls()
    {
        EXPECT_CALL(*textureMock, getSize()).WillRepeatedly(Return(t_buttonSize));
    }

    void expectGetFontTexture(const unsigned int times = 1)
    {
        EXPECT_CALL(*fontMock, getTexture()).Times(times).WillRepeatedly(Return(texture));
    }

    void expectGetGlyphRepeatedly()
    {
        EXPECT_CALL(*fontMock, getGlyph(_, _, _)).WillRepeatedly(ReturnRef(t_glyph1));
    }

    void expectedUpdateVertexArray(const unsigned int times = 1)
    {
        expectGetFontTexture(times);
        EXPECT_CALL(*textureMock, getTextureId()).Times(times).WillRepeatedly(Return(1));
        EXPECT_CALL(*fontMock, getUnderlinePosition()).WillRepeatedly(Return(1.f));
        EXPECT_CALL(*fontMock, getUnderlineThickness()).WillRepeatedly(Return(2.f));
        expectGetGlyphRepeatedly();
        EXPECT_CALL(*fontMock, getLineSpacing()).WillRepeatedly(Return(1.f));
        EXPECT_CALL(*fontMock, getKerning(_, _, _)).WillRepeatedly(Return(1.f));
        EXPECT_CALL(*textureMock, getSize()).WillRepeatedly(Return(geometry::Size2Dpxl{128, 128}));
        EXPECT_CALL(*fontMock, getCharacterSize()).Times(times*2).WillRepeatedly(Return(t_defaultCharacterSize));
    }

    std::unique_ptr<he::gfx::draw::Button> sut;
    std::shared_ptr<he::gfx::text::FontMock> fontMock = std::make_shared<he::gfx::text::FontMock>();

    std::shared_ptr<he::gfx::render::TextureMock> textureMock = std::make_shared<he::gfx::render::TextureMock>();
    std::shared_ptr<he::gfx::render::ITexture> textureBase = textureMock;
    std::shared_ptr<he::gfx::render::Texture> texture = std::static_pointer_cast<he::gfx::render::Texture>(textureBase);
};

class ButtonTests : public testing::Test , public ButtonTestsFixture
{
public:
    ButtonTests() = default;
    ~ButtonTests() override {};
};

TEST_F(ButtonTests, construction_whenCreate_shouldCreateWithoutError)
{
    expectInitCalls();

    createSut();
}

TEST_F(ButtonTests, setCallback_whenSetCallbackAndRemove_shouldReturnNothing)
{
    expectInitCalls();
    he::window::Event::MouseButtonAction mouseButtonAction{he::window::Mouse::Button::Left, 50, 50};
    he::window::Event event(he::window::Event::mouseButtonPressed, mouseButtonAction);
    unsigned int numberToSet{0};    

    createSut();
    sut->setCallback([&numberToSet](){numberToSet=123;}, event);
    sut->removeCallback(event);

    ASSERT_EQ(sut->onMouseButtonPressed(mouseButtonAction), false);
    ASSERT_EQ(numberToSet, 0);
}

TEST_F(ButtonTests, setCallback_whenSetDifferentCallbacksAndRemoveChoosen_shouldCallOnlyExistedCallbacks)
{
    expectInitCalls();
    he::window::Event::MouseButtonAction mouseButtonAction1{he::window::Mouse::Button::Left, 50, 50};
    he::window::Event::MouseButtonAction mouseButtonAction2{he::window::Mouse::Button::Right, 50, 50};
    he::window::Event::MouseMoveEvent mouseButtonAction3{50, 50};
    he::window::Event eventMouseButtonPressedLeft(window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}));
    he::window::Event eventMouseButtonPressedRight(window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Right}));
    he::window::Event eventMouseCursorMoved(window::Event(window::Event::mouseCursorMoved));
    char charToSet{'0'};    

    createSut();
    sut->setCallback([&charToSet](){charToSet='L';}, eventMouseButtonPressedLeft);
    sut->setCallback([&charToSet](){charToSet='R';}, eventMouseButtonPressedRight);
    sut->setCallback([&charToSet](){charToSet='M';}, eventMouseCursorMoved);
    sut->removeCallback(eventMouseButtonPressedRight);
    sut->removeCallback(eventMouseCursorMoved);

    ASSERT_EQ(sut->onMouseButtonPressed(mouseButtonAction1), true);
    ASSERT_EQ(charToSet, 'L');
    ASSERT_EQ(sut->onMouseButtonPressed(mouseButtonAction2), false);
    ASSERT_EQ(sut->onMouseCursorMoved(mouseButtonAction3), false);
}

TEST_F(ButtonTests, setCallback_whenSetCallbackTwiceForTheSameButton_shouldRemoveFirstAndAddSecond)
{
    expectInitCalls();
    he::window::Event::MouseButtonAction mouseButtonAction{he::window::Mouse::Button::Left, 50, 50};
    he::window::Event event(he::window::Event::mouseButtonPressed, mouseButtonAction);
    unsigned int numberToSet{0};    

    createSut();
    sut->setCallback([&numberToSet](){numberToSet=123;}, event);
    sut->setCallback([&numberToSet](){numberToSet=125;}, event);

    ASSERT_EQ(sut->onMouseButtonPressed(mouseButtonAction), true);
    ASSERT_EQ(numberToSet, 125);
}

class ButtonMauseButtonPressedEventTests : public testing::TestWithParam<std::tuple<he::window::Event::MouseButtonAction, bool, he::window::Event, bool>> , public ButtonTestsFixture
{
};

TEST_P(ButtonMauseButtonPressedEventTests, onMouseButtonPressed_whenMouseEventCome_shouldDoAction)
{
    const auto [p_event, p_needSetCallback , p_callbackEvent, p_expectedResult] = GetParam();
    expectInitCalls();

    createSut();
    if (p_needSetCallback)
    {
        sut->setCallback([](){}, p_callbackEvent);
    }

    ASSERT_EQ(sut->onMouseButtonPressed(p_event), p_expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    ButtonTests, 
    ButtonMauseButtonPressedEventTests,
    testing::Values(
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Left, 50, 50}, true,
            window::Event(window::Event::mouseButtonPressed), false),
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Left, 50, 50}, true,
            window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}), true),
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Left, 101, 101}, true,
            window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}), false),
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Left, 50, 50}, false,
            window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}), false),
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Left, 101, 101}, false,
            window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}), false),
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Right, 50, 50}, true,
            window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Right}), true),
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Right, 50, 50}, true,
            window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Left}), false),
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Middle, 50, 50}, true,
            window::Event(window::Event::mouseButtonPressed, window::Event::MouseButtonAction{window::Mouse::Button::Middle}), true)));

class ButtonMauseButtonReleaseEventTests : public testing::TestWithParam<std::tuple<he::window::Event::MouseButtonAction, bool, he::window::Event, bool>> , public ButtonTestsFixture
{
};

TEST_P(ButtonMauseButtonReleaseEventTests, onMouseButtonReleased_whenMouseEventCome_shouldDoAction)
{
    const auto [p_event, p_needSetCallback , p_callbackEvent, p_expectedResult] = GetParam();
    expectInitCalls();

    createSut();
    if (p_needSetCallback)
    {
        sut->setCallback([](){}, p_callbackEvent);
    }

    ASSERT_EQ(sut->onMouseButtonReleased(p_event), p_expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    ButtonTests, 
    ButtonMauseButtonReleaseEventTests,
    testing::Values(
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Left, 50, 50}, true,
            window::Event(window::Event::mouseButtonReleased), false),
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Left, 50, 50}, true,
            window::Event(window::Event::mouseButtonReleased, window::Event::MouseButtonAction{window::Mouse::Button::Left}), true),
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Left, 101, 101}, true,
            window::Event(window::Event::mouseButtonReleased, window::Event::MouseButtonAction{window::Mouse::Button::Left}), false),
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Left, 50, 50}, false,
            window::Event(window::Event::mouseButtonReleased, window::Event::MouseButtonAction{window::Mouse::Button::Left}), false),
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Left, 101, 101}, false,
            window::Event(window::Event::mouseButtonReleased, window::Event::MouseButtonAction{window::Mouse::Button::Left}), false),
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Right, 50, 50}, true,
            window::Event(window::Event::mouseButtonReleased, window::Event::MouseButtonAction{window::Mouse::Button::Right}), true),
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Right, 50, 50}, true,
            window::Event(window::Event::mouseButtonReleased, window::Event::MouseButtonAction{window::Mouse::Button::Left}), false),
        std::make_tuple(window::Event::MouseButtonAction{window::Mouse::Button::Middle, 50, 50}, true,
            window::Event(window::Event::mouseButtonReleased, window::Event::MouseButtonAction{window::Mouse::Button::Middle}), true)));

class ButtonMauseCursorMovedEventTests : public testing::TestWithParam<std::tuple<he::window::Event::MouseMoveEvent, bool, he::window::Event, bool>> , public ButtonTestsFixture
{
};

TEST_P(ButtonMauseCursorMovedEventTests, onMauseCursorMoved_whenMouseEventCome_shouldDoAction)
{
    const auto [p_event, p_needSetCallback , p_callbackEvent, p_expectedResult] = GetParam();
    expectInitCalls();

    createSut();
    if (p_needSetCallback)
    {
        sut->setCallback([](){}, p_callbackEvent);
    }

    ASSERT_EQ(sut->onMouseCursorMoved(p_event), p_expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    ButtonTests, 
    ButtonMauseCursorMovedEventTests,
    testing::Values(
        std::make_tuple(window::Event::MouseMoveEvent{50, 50}, true,
            window::Event(window::Event::mouseCursorMoved), true),
        std::make_tuple(window::Event::MouseMoveEvent{50, 50}, false,
            window::Event(window::Event::mouseCursorMoved), false),
        std::make_tuple(window::Event::MouseMoveEvent{101, 101}, true,
            window::Event(window::Event::mouseCursorMoved), false),
        std::make_tuple(window::Event::MouseMoveEvent{101, 101}, false,
            window::Event(window::Event::mouseCursorMoved), false)));

TEST_F(ButtonTests, setOriginInCenter_whenSetOriginInCenterAfterSetPosition_shouldTextBeInCenterButton)
{
    std::unique_ptr<he::gfx::draw::Text> text = std::make_unique<he::gfx::draw::TextMock>("text_1", fontMock);
    gfx::geometry::Point2Df position{200.f, 200.f};
    expectInitCalls();
    expectedUpdateVertexArray(2);

    createSut();
    sut->setText(*text);
    sut->setPosition(position);
    sut->setOriginInCenter();
}


} // namespace he::gfx::draw