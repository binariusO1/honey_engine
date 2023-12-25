#include <gtest/gtest.h>
#include "TextFixture.hpp"
#include "draw/TextMock.hpp"
#include "gfx/draw/AdvButton.hpp"
#include "graphic/TextureMock.hpp"
#include "text/FontMock.hpp"

using namespace ::testing;

namespace
{
constexpr he::gfx::geometry::Size2Dpxl t_buttonSize{100, 100};
} // namespace

namespace he::gfx::draw
{
class AdvButtonTestsFixture : public text::TextFixture
{
public:
    void createSut()
    {
        sut = std::make_unique<he::gfx::draw::AdvButton>("button1", texture);
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

    std::unique_ptr<he::gfx::draw::AdvButton> sut;
    std::shared_ptr<he::gfx::text::FontMock> fontMock = std::make_shared<he::gfx::text::FontMock>();

    std::shared_ptr<he::gfx::render::TextureMock> textureMock = std::make_shared<he::gfx::render::TextureMock>();
    std::shared_ptr<he::gfx::render::ITexture> textureBase = textureMock;
    std::shared_ptr<he::gfx::render::Texture> texture = std::static_pointer_cast<he::gfx::render::Texture>(textureBase);
};

class AdvButtonTests : public testing::Test , public AdvButtonTestsFixture
{
public:
    AdvButtonTests() = default;
    ~AdvButtonTests() override {};
};

TEST_F(AdvButtonTests, construction_whenCreate_shouldCreateWithoutError)
{
    expectInitCalls();
    createSut();
}

TEST_F(AdvButtonTests, onMouseCursorMoved_whenButonIsOnClickedStateAndMouseCursorIsMoved_shouldNotTriggerCallbackIfNotSet)
{
    expectInitCalls();
    he::window::Event::MouseButtonAction mouseButtonAction{he::window::Mouse::Button::Left, 50, 50};
    he::window::Event::MouseMoveEvent mouseMoveEvent1{50, 50};
    he::window::Event::MouseMoveEvent mouseMoveEvent2{52, 52};

    createSut();
    int callbackTrigger{0};
    he::gfx::draw::ButtonCallback callbackTouchedClicked = [this, &callbackTrigger](){
        callbackTrigger++;
    };
    sut->setCallback(callbackTouchedClicked, {ButtonState::Touched, ButtonState::Clicked});
    sut->onMouseCursorMoved(mouseMoveEvent1);
    sut->onMouseButtonPressed(mouseButtonAction);
    sut->onMouseCursorMoved(mouseMoveEvent2);

    ASSERT_EQ(callbackTrigger, 1);
}
} // namespace he::gfx::draw