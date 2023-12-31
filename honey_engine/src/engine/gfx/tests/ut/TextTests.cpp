#include <gtest/gtest.h>
#include "TextFixture.hpp"
#include "draw/TextMock.hpp"
#include "gfx/draw/Text.hpp"
#include "gfx/text/Font.hpp"
#include "graphic/TextureMock.hpp"
#include "text/FontMock.hpp"

using namespace ::testing;

namespace  he::gfx::draw
{
namespace
{
const std::string t_string1{"string1"};
const std::string t_string2{"string2"};
const he::gfx::text::Font t_defaultFont;

} // namespace

class TextTests : public testing::Test , public text::TextFixture
{
public:
    TextTests() = default;
    ~TextTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::draw::TextMock>("text_1", fontMock);
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

    std::unique_ptr<he::gfx::draw::Text> sut;
    std::shared_ptr<he::gfx::text::FontMock> fontMock = std::make_shared<he::gfx::text::FontMock>();

    std::shared_ptr<he::gfx::render::TextureMock> textureMock = std::make_shared<he::gfx::render::TextureMock>();
    std::shared_ptr<he::gfx::render::ITexture> textureBase = textureMock;
    std::shared_ptr<he::gfx::render::Texture> texture = std::static_pointer_cast<he::gfx::render::Texture>(textureBase);
};


TEST_F(TextTests, whenCreate_shouldCreateWithoutError)
{
    createSut();
}

TEST_F(TextTests, copyConstructor_whenCopyObject_shouldCopyWithoutError)
{
    // TODO
}

TEST_F(TextTests, setString_whenCharacterSizeIsNotSet_shouldSetStringAndNotUpdateVertexArray)
{   
    EXPECT_CALL(*fontMock, getCharacterSize()).WillOnce(Return(0));

    createSut();
    sut->setString(t_string1);
    sut->update();

    ASSERT_EQ(sut->getString(), t_string1);
}

TEST_F(TextTests, setString_whenSetNewString_shouldSetString)
{
    EXPECT_CALL(*fontMock, setCharacterSize(t_defaultCharacterSize)).WillRepeatedly(Return(true));

    createSut();
    sut->setCharacterSize(t_defaultCharacterSize);
    sut->setString(t_string1);

    ASSERT_EQ(sut->getString(), t_string1);
}

TEST_F(TextTests, setString_whenSetSecondNewString_shouldSetNewString)
{
    EXPECT_CALL(*fontMock, setCharacterSize(t_defaultCharacterSize)).WillRepeatedly(Return(true));

    createSut();
    sut->setCharacterSize(t_defaultCharacterSize);
    sut->setString(t_string1);
    sut->setString(t_string2);

    ASSERT_EQ(sut->getString(), t_string2);
}

TEST_F(TextTests, setString_whenSetSeconSameString_shouldNotSetStringAndShouldUpdateVertexArrayOnce)
{
    EXPECT_CALL(*fontMock, setCharacterSize(t_defaultCharacterSize)).WillRepeatedly(Return(true));
    expectedUpdateVertexArray();

    createSut();
    sut->setCharacterSize(t_defaultCharacterSize);
    sut->setFont(t_defaultFont);
    sut->setString(t_string1);
    sut->setString(t_string1);
    sut->update();
}

TEST_F(TextTests, setFont_whenSetFont_shouldSetFont)
{
    EXPECT_CALL(*fontMock, setCharacterSize(t_defaultCharacterSize)).WillRepeatedly(Return(true));

    createSut();
    sut->setCharacterSize(t_defaultCharacterSize);
    sut->setFont(t_defaultFont);
}

TEST_F(TextTests, setColor_whenSetColorToBlack_shouldSetColorAndNotUpdateVertexArray)
{
    EXPECT_CALL(*fontMock, setCharacterSize(t_defaultCharacterSize)).WillRepeatedly(Return(true));

    createSut();
    sut->setCharacterSize(t_defaultCharacterSize);
    sut->setString(t_string1);
    sut->setColor(gfx::Color::Black);

    for (const auto& vertex : sut->getVertexArray())
    {
        ASSERT_EQ(vertex.color, gfx::Color::Black);
    }
}

TEST_F(TextTests, setStyle_whenSetStyle_shouldSetAndGetProperValue)
{
    EXPECT_CALL(*fontMock, setCharacterSize(t_defaultCharacterSize)).WillRepeatedly(Return(true));

    createSut();
    sut->setCharacterSize(t_defaultCharacterSize);
    sut->setStyle(text::FontStyle::Bold | text::FontStyle::Italic);

    ASSERT_EQ(sut->getStyle(), 3);
}

TEST_F(TextTests, setOrigin_whenSetOrigin_shouldSetProperValue)
{
    EXPECT_CALL(*fontMock, setCharacterSize(t_defaultCharacterSize)).WillRepeatedly(Return(true));

    createSut();
    sut->setCharacterSize(t_defaultCharacterSize);
    sut->setOrigin({1.f, 2.f});

    ASSERT_EQ(sut->getOrigin(), he::gfx::geometry::Point2Df(1.f, 2.f));
}

TEST_F(TextTests, setOriginInCenter_whenSetOrigin_shouldSetProperValue)
{
    EXPECT_CALL(*fontMock, setCharacterSize(t_defaultCharacterSize)).WillRepeatedly(Return(true));

    createSut();
    sut->setCharacterSize(t_defaultCharacterSize);
    sut->setString(t_string1);
    sut->setOriginInCenter();

    auto centerX = sut->getLocalBounds().width/2.f;
    auto centerY = sut->getLocalBounds().height/2.f;

    ASSERT_EQ(sut->getOrigin().x, centerX);
    ASSERT_EQ(sut->getOrigin().y, centerY);
}

TEST_F(TextTests, setPosition_whenSetPosition_shouldSetPosition)
{
    he::gfx::geometry::Point2Df point{123, 20};
    EXPECT_CALL(*fontMock, setCharacterSize(t_defaultCharacterSize)).WillRepeatedly(Return(true));

    createSut();
    sut->setCharacterSize(t_defaultCharacterSize);
    sut->setString(t_string1);
    sut->setPosition(point);

    ASSERT_EQ(sut->getPosition(), point);
}

TEST_F(TextTests, setPosition_whenSetNewPosition_pointsShouldBeChanged)
{
    const he::gfx::geometry::Point2Df position{150.0, 140.0};
    EXPECT_CALL(*fontMock, setCharacterSize(t_defaultCharacterSize)).WillRepeatedly(Return(true));
    expectedUpdateVertexArray();

    createSut();
    sut->setCharacterSize(t_defaultCharacterSize);
    sut->setString(t_string1);

    auto vertexArray = sut->getVertexArray();

    sut->setPosition(position);
    sut->update();

    for (size_t i = 0 ; i < vertexArray.size() ; ++i)
    {
        ASSERT_NE(sut->getVertexArray()[i].position, vertexArray[i].position);
    }
}

} // namespace he::gfx::draw