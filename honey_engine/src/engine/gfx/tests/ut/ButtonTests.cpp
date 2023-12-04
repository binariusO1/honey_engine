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
class ButtonTests : public testing::Test , public text::TextFixture
{
public:
    ButtonTests() = default;
    ~ButtonTests() override {};

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

TEST_F(ButtonTests, construction_whenCreate_shouldCreateWithoutError)
{
    expectInitCalls();

    createSut();
}

TEST_F(ButtonTests, setCallback_whenSetCallbackAndRemove_shouldReturnNothing)
{
    expectInitCalls();
    he::window::Event::MouseButtonAction event{he::window::Mouse::Button::Left, 50, 50};
    unsigned int numberToSet{0};    

    createSut();
    sut->setCallback([&numberToSet](){numberToSet=123;}, window::Mouse::Button::Left);
    sut->removeCallback(window::Mouse::Button::Left);

    ASSERT_EQ(sut->onMauseButtonPressed(event), false);
    ASSERT_EQ(numberToSet, 0);
}

TEST_F(ButtonTests, setCallback_whenSetCallbackTwiceForTheSameButton_shouldRemoveFirstAndAddSecond)
{
    expectInitCalls();
    he::window::Event::MouseButtonAction event{he::window::Mouse::Button::Left, 50, 50};
    unsigned int numberToSet{0};    

    createSut();
    sut->setCallback([&numberToSet](){numberToSet=123;}, window::Mouse::Button::Left);
    sut->setCallback([&numberToSet](){numberToSet=125;}, window::Mouse::Button::Left);

    ASSERT_EQ(sut->onMauseButtonPressed(event), true);
    ASSERT_EQ(numberToSet, 125);
}

TEST_F(ButtonTests, onMauseButtonPressed_whenButtonPressedInsideAndCallbackButtonDefined_shouldReturnTrue)
{
    expectInitCalls();
    he::window::Event::MouseButtonAction event{he::window::Mouse::Button::Left, 1, 99};

    createSut();
    sut->setCallback([](){}, window::Mouse::Button::Left);

    ASSERT_EQ(sut->onMauseButtonPressed(event), true);
}

TEST_F(ButtonTests, onMauseButtonPressed_whenButtonPressedOutsideAndCallbackButtonDefined_shouldReturnFalse)
{
    expectInitCalls();
    he::window::Event::MouseButtonAction event{he::window::Mouse::Button::Left, 101, 101};

    createSut();
    sut->setCallback([](){}, window::Mouse::Button::Left);

    ASSERT_EQ(sut->onMauseButtonPressed(event), false);
}

TEST_F(ButtonTests, onMauseButtonPressed_whenButtonPressedInside_shouldReturnFalse)
{
    expectInitCalls();
    he::window::Event::MouseButtonAction event{he::window::Mouse::Button::Left, 1, 99};

    createSut();
    
    ASSERT_EQ(sut->onMauseButtonPressed(event), false);
}

TEST_F(ButtonTests, onMauseButtonPressed_whenButtonPressedOutside_shouldReturnFalse)
{
    expectInitCalls();
    he::window::Event::MouseButtonAction event{he::window::Mouse::Button::Left, 101, 101};

    createSut();
    
    ASSERT_EQ(sut->onMauseButtonPressed(event), false);
}

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