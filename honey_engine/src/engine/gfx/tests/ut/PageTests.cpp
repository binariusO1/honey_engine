#include <gtest/gtest.h>
#include "TextFixture.hpp"
#include "graphic/TextureMock.hpp"

using namespace ::testing;

namespace he::gfx::text
{
namespace
{
} // namespace

class PageTestsWrapper : public Page
{
public:
    PageTestsWrapper(std::shared_ptr<he::gfx::render::ITexture> ptr) : Page()
    {
        m_texture = ptr;
    }
};


class PageTests : public testing::Test , public text::TextFixture
{
public:
    PageTests() = default;
    ~PageTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::text::PageTestsWrapper>(textureMock);
    }

    void expectInit()
    {
        EXPECT_CALL(*textureMock, create(_, _)).Times(1);
        EXPECT_CALL(*textureMock, setSmooth(_)).Times(1);
    }

    std::unique_ptr<he::gfx::text::PageTestsWrapper> sut;
    std::shared_ptr<he::gfx::render::TextureMock> textureMock = std::make_shared<he::gfx::render::TextureMock>();
};

TEST_F(PageTests, whenCreate_shouldCreateAndInitWithoutError)
{
    expectInit();

    createSut();
    sut->createTextureWithoutSmooth();

    ASSERT_NE(sut->getTexture(), nullptr);
}

TEST_F(PageTests, afterPopulateGlyphTable_shouldGetGlyph)
{
    expectInit();
    std::uint64_t key1{1};

    createSut();
    sut->createTextureWithSmooth();
    sut->addGlyph(key1, t_glyph1);

    ASSERT_EQ(sut->getGlyphTable().at(key1).advance, t_glyph1.advance);
    ASSERT_EQ(sut->getGlyphTable().at(key1).bounds.p1, t_glyph1.bounds.p1);
}

TEST_F(PageTests, afterPopulateGlyphTable_shouldGetOnlyFirstGlyph)
{
    expectInit();
    std::uint64_t key1{1};

    createSut();
    sut->createTextureWithSmooth();
    sut->addGlyph(key1, t_glyph1);
    sut->addGlyph(key1, t_glyph2);

    ASSERT_EQ(sut->getGlyphTable().at(key1).advance, t_glyph1.advance);
    ASSERT_EQ(sut->getGlyphTable().at(key1).lsbDelta, t_glyph1.lsbDelta);
}

TEST_F(PageTests, afterPopulateRows_shouldGetRow)
{
    expectInit();
    const unsigned int height1{10};
    const unsigned int height2{12};
    const unsigned int height3{15};

    createSut();
    sut->createTextureWithSmooth();
    sut->addRowHeight(height1);
    sut->addRowHeight(height2);
    sut->addRowHeight(height3);

    ASSERT_EQ(sut->getRows()[0].height, height1);
    ASSERT_EQ(sut->getRows()[0].width, 0);
    ASSERT_EQ(sut->getRows()[2].top, sut->getPositionOfNextRow());
}

TEST_F(PageTests, afterAddRow_shouldSetWidthForRow)
{
    expectInit();
    const unsigned int height1{15};
    const unsigned int width1{7};

    createSut();
    sut->createTextureWithSmooth();
    sut->addRowHeight(height1);
    sut->setWidthForRow(0, 7);

    ASSERT_EQ(sut->getRows()[0].width, width1);
}

TEST_F(PageTests, afterGetRowWhichIsNotExist_shouldNotSetWidth)
{
    expectInit();

    createSut();
    sut->createTextureWithSmooth();

    EXPECT_FALSE(sut->setWidthForRow(2, 9));
}
} // namespace he::gfx::draw