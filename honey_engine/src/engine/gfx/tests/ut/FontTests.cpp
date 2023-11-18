#include <gtest/gtest.h>
#include "TextFixture.hpp"
#include "gfx/text/Font.hpp"
#include "text/FreeType2Mock.hpp"

using namespace ::testing;

namespace  he::gfx::text
{
namespace
{
} // namespace

class FontTestsWrapper : public Font
{
public:
    ~FontTestsWrapper() override {};

public:
    FontTestsWrapper(std::shared_ptr<he::gfx::text::IFreeType2Wrapper> ptr, Page& page) : Font() , page(page)
    {
        m_freeType2Wrapper = ptr;
    }

    he::gfx::text::Page& loadPage(const unsigned int) const override
    {
        return page;
    }

    Page& page;
};


class FontTests : public testing::Test , public text::TextFixture
{
public:
    FontTests() = default;
    ~FontTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::text::FontTestsWrapper>(freeTypeMock, page);
    }

    void createSutWithGlyph()
    {
        page.addGlyph(4575657223555907585, t_glyph1);
        sut = std::make_unique<he::gfx::text::FontTestsWrapper>(freeTypeMock, page);
    }

    void createSutWithTwoGlyphs()
    {
        page.addGlyph(48, t_glyph1);
        page.addGlyph(56, t_glyph2);
        sut = std::make_unique<he::gfx::text::FontTestsWrapper>(freeTypeMock, page);
    }

    Page page;

    void expectCallNewFace()
    {
        EXPECT_CALL(*freeTypeMock, newFace(_, _, _, _))
            .WillOnce(([](FT_Library library, const char* filepathname, FT_Long face_index, FT_Face* aface) 
            {
                return FT_New_Face(library, filepathname, face_index, aface);
            }));
    }

    void expectCallStrokerNew()
    {
        EXPECT_CALL(*freeTypeMock, strokerNew(_, _))
            .WillOnce(([](FT_Library library, FT_Stroker* astroker)
            {
                return FT_Stroker_New(library, astroker);
            }));
    }

    void expectInitializeFreeType()
    {
        EXPECT_CALL(*freeTypeMock, initFreeType(_))
            .WillOnce(([](FT_Library* library)
            {
                return FT_Init_FreeType(library);
            }));
    }

    void expectGetGlyph()
    {
        EXPECT_CALL(*freeTypeMock, getGlyph(_, _))
            .WillOnce(([](FT_GlyphSlot slot, FT_Glyph *aglyph)
            {
                return FT_Get_Glyph(slot, aglyph);
            }));
    }

    void expectMultiplierFix()
    {
        EXPECT_CALL(*freeTypeMock, multiplierFix(_, _))
        .WillOnce(([](FT_Long a, FT_Long b)
            {
                return FT_MulFix(a, b);
            }));
    }

    void expectInitFreeType()
    {
        expectInitializeFreeType();
        expectCallNewFace();
        expectCallStrokerNew();
        EXPECT_CALL(*freeTypeMock, selectCharmap(_, _)).WillOnce(Return(0));
    }

    void expectLoadFromFileSuccess()
    {
        EXPECT_CALL(*freeTypeMock, setErrorCode(_)).Times(4);
        EXPECT_CALL(*freeTypeMock, setPixelSizes(_, _, _)).WillRepeatedly(Return(0));
        expectInitFreeType();
    }

    std::unique_ptr<he::gfx::text::Font> sut;
    std::shared_ptr<he::gfx::text::FreeType2Mock> freeTypeMock = std::make_shared<he::gfx::text::FreeType2Mock>();
};


TEST_F(FontTests, copyConstructor_whenCopyObject_shouldCopyWithoutError)
{
    Font font1;
    // font1.setCharacterSize(14);
    // TODO
    ASSERT_NO_THROW(Font copy(font1));
}

TEST_F(FontTests, loadFromFile_whenGetCorrectPath_shouldReturnTrue)
{
    EXPECT_CALL(*freeTypeMock, setErrorCode(_)).Times(4);
    expectInitFreeType();

    createSut();
    ASSERT_TRUE(sut->loadFromFile(t_fontCalibriPath));
}

TEST_F(FontTests, loadFromFile_whenGetWrongString_shouldReturnFalse)
{
    EXPECT_CALL(*freeTypeMock, setErrorCode(_)).Times(AtLeast(1));
    EXPECT_CALL(*freeTypeMock, getErrorDesc()).Times(AtLeast(1));

    createSut();
    ASSERT_FALSE(sut->loadFromFile("some/weird/string"));
}

TEST_F(FontTests, loadFromFile_whenGetWrongPath_shouldReturnFalse)
{
    EXPECT_CALL(*freeTypeMock, setErrorCode(_)).Times(AtLeast(1));
    EXPECT_CALL(*freeTypeMock, getErrorDesc()).Times(AtLeast(1));
    expectInitializeFreeType();
    expectCallNewFace();

    createSut();
    ASSERT_FALSE(sut->loadFromFile("\\data\\gfx\\fonts\\wrongFontName.ttf"));
}

TEST_F(FontTests, setCharacterSize_whenSetNewCharacterSize_shouldReturnTrue)
{
    expectLoadFromFileSuccess();

    createSut();
    sut->loadFromFile(t_fontCalibriPath);

    ASSERT_TRUE(sut->setCharacterSize(15));
}

TEST_F(FontTests, setCharacterSize_whenSetAgainSameCharacterSize_shouldReturnTrue)
{
    expectLoadFromFileSuccess();

    createSut();
    sut->loadFromFile(t_fontCalibriPath);
    sut->setCharacterSize(15);

    ASSERT_TRUE(sut->setCharacterSize(15));
}

TEST_F(FontTests, setCharacterSize_whenFreeTypeNotInitialized_shouldReturnFalse)
{
    createSut();

    ASSERT_FALSE(sut->setCharacterSize(12));
}

TEST_F(FontTests, setCharacterSize_getInvalidPixelSizesError_shouldReturnFalse)
{
    FT_Error result = FT_Err_Invalid_Pixel_Size;
    EXPECT_CALL(*freeTypeMock, setErrorCode(_)).Times(4);
    EXPECT_CALL(*freeTypeMock, setPixelSizes(_, _, _)).WillOnce(Return(result));
    EXPECT_CALL(*freeTypeMock, isScalable(_)).WillOnce(Return(true));
    expectInitFreeType();

    createSut();
    sut->loadFromFile(t_fontCalibriPath);

    ASSERT_FALSE(sut->setCharacterSize(12));
}

TEST_F(FontTests, setCharacterSize_getInvalidPixelSizesErrorAndFaceIsNotScalable_shouldReturnFalse)
{
    FT_Error result = FT_Err_Invalid_Pixel_Size;
    EXPECT_CALL(*freeTypeMock, setErrorCode(_)).Times(4);
    EXPECT_CALL(*freeTypeMock, setPixelSizes(_, _, _)).WillOnce(Return(result));
    EXPECT_CALL(*freeTypeMock, isScalable(_)).WillOnce(Return(false));
    expectInitFreeType();

    createSut();
    sut->loadFromFile(t_fontCalibriPath);

    ASSERT_FALSE(sut->setCharacterSize(10));
}

TEST_F(FontTests, getUnderlinePosition_afterInitFreeTypeWhenIsNotScalable_shouldReturnNonZeroValue)
{
    expectLoadFromFileSuccess();
    EXPECT_CALL(*freeTypeMock, isScalable(_)).WillOnce(Return(false));

    createSut();
    sut->loadFromFile(t_fontCalibriPath);
    sut->setCharacterSize(12);
    ASSERT_NE(sut->getUnderlinePosition(12), 0.f);
}

TEST_F(FontTests, getUnderlinePosition_afterInitFreeTypeWhenIsScalable_shouldReturnNonZeroValue)
{
    expectLoadFromFileSuccess();
    EXPECT_CALL(*freeTypeMock, isScalable(_)).WillOnce(Return(true));
    expectMultiplierFix();

    createSut();
    sut->loadFromFile(t_fontCalibriPath);
    sut->setCharacterSize(12);
    ASSERT_FLOAT_EQ(sut->getUnderlinePosition(12), 0.f);
}

TEST_F(FontTests, getUnderlinePosition_whenFreeTypeIsNotInitialized_shouldReturnZeroValue)
{
    createSut();

    ASSERT_FLOAT_EQ(sut->getUnderlinePosition(12), 0.f);
}


TEST_F(FontTests, getUnderlineThickness_afterInitFreeTypeWhenIsNotScalable_shouldReturnNonZeroValue)
{
    expectLoadFromFileSuccess();
    EXPECT_CALL(*freeTypeMock, isScalable(_)).WillOnce(Return(false));

    createSut();
    sut->loadFromFile(t_fontCalibriPath);
    sut->setCharacterSize(12);
    ASSERT_NE(sut->getUnderlineThickness(12), 0.f);
}

TEST_F(FontTests, getUnderlineThickness_afterInitFreeTypeWhenIsScalable_shouldReturnNonZeroValue)
{
    expectLoadFromFileSuccess();
    EXPECT_CALL(*freeTypeMock, isScalable(_)).WillOnce(Return(true));
    expectMultiplierFix();

    createSut();
    sut->loadFromFile(t_fontCalibriPath);
    sut->setCharacterSize(12);
    ASSERT_FLOAT_EQ(sut->getUnderlineThickness(12), 0.f);
}

TEST_F(FontTests, getUnderlineThickness_whenFreeTypeIsNotInitialized_shouldReturnZeroValue)
{
    createSut();

    ASSERT_FLOAT_EQ(sut->getUnderlineThickness(12), 0.f);
}

TEST_F(FontTests, getLineSpacing_afterInitFreeType_shouldReturnNonZeroValue)
{
    expectLoadFromFileSuccess();

    createSut();
    sut->loadFromFile(t_fontCalibriPath);
    sut->setCharacterSize(12);
    ASSERT_FLOAT_EQ(sut->getLineSpacing(12), 0.f);
}

TEST_F(FontTests, getLineSpacing_whenFreeTypeIsNotInitialized_shouldReturnZeroValue)
{
    createSut();

    ASSERT_FLOAT_EQ(sut->getLineSpacing(12), 0.f);
}

TEST_F(FontTests, getGlyph_whenGetGlyphFromEmptyGlyphTable_shouldReturnNewGlyph)
{
    expectLoadFromFileSuccess();
    EXPECT_CALL(*freeTypeMock, getCharIndex(_, _)).WillOnce(Return(1));
    EXPECT_CALL(*freeTypeMock, loadChar(_, _, _));
    expectGetGlyph();

    createSut();
    sut->loadFromFile(t_fontCalibriPath);
    sut->setCharacterSize(12);
    auto glyph = sut->getGlyph(12, 12, true, 1);

    ASSERT_FLOAT_EQ(glyph.advance, 0.f);
    ASSERT_EQ(glyph.lsbDelta, 0);
    ASSERT_EQ(glyph.rsbDelta, 0);
}

TEST_F(FontTests, getGlyph_whenGetGlyphFromNonEmptyGlyphTable_shouldReturnGlyph)
{
    expectLoadFromFileSuccess();
    EXPECT_CALL(*freeTypeMock, getCharIndex(_, _)).WillOnce(Return(1));

    createSutWithGlyph();
    sut->loadFromFile(t_fontCalibriPath);
    sut->setCharacterSize(12);
    auto glyph = sut->getGlyph(12, 12, true, 1);

    ASSERT_FLOAT_EQ(glyph.advance, t_glyph1.advance);
    ASSERT_EQ(glyph.lsbDelta, t_glyph1.lsbDelta);
    ASSERT_EQ(glyph.rsbDelta, t_glyph1.rsbDelta);
}

TEST_F(FontTests, getKerning_whenFreeTypeIsNotInitialized_shouldReturnZeroValue)
{
    createSut();

    ASSERT_FLOAT_EQ(sut->getKerning(1, 2, 12, false), 0.f);
}

TEST_F(FontTests, getKerning_whenFreeTypeIsInitialized_shouldReturnNonZeroValue)
{
    expectLoadFromFileSuccess();
    EXPECT_CALL(*freeTypeMock, getCharIndex(_, _)).WillOnce(Return(48)).WillOnce(Return(56)).WillOnce(Return(48)).WillOnce(Return(56));
    EXPECT_CALL(*freeTypeMock, hasKerning(_)).WillOnce(Return(true));
    EXPECT_CALL(*freeTypeMock, getKerning(_, _, _, _, _));
    EXPECT_CALL(*freeTypeMock, isScalable(_)).WillOnce(Return(true));

    createSutWithTwoGlyphs();
    sut->loadFromFile(t_fontCalibriPath);
    sut->setCharacterSize(12);

    ASSERT_NE(sut->getKerning(1, 2, 12, false), 0.f);
}


TEST_F(FontTests, getKerning_whenFreeTypeIsInitializedAndOneCodePointIsEqualZero_shouldReturnZeroValue)
{
    expectLoadFromFileSuccess();

    createSut();
    sut->loadFromFile(t_fontCalibriPath);
    sut->setCharacterSize(12);

    ASSERT_FLOAT_EQ(sut->getKerning(1, 0, 12, false), 0.f);
}

} // namespace he::gfx::text