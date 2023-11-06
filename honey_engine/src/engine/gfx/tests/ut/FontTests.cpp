#include <gtest/gtest.h>
#include "exception/invalid_initialization.hpp"
#include "gfx/text/Font.hpp"

namespace
{
} // namespace
namespace  he::gfx::text
{
class FontTests : public testing::Test
{
public:
    FontTests() = default;
    ~FontTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::text::Font>();
    }

    std::unique_ptr<he::gfx::text::Font> sut;
};

TEST_F(FontTests, whenCreatedDefault_shouldCreateWithoutError)
{
    createSut();
}

TEST_F(FontTests, whenCreatedCustomFonts_shouldCreateWithoutError)
{
    Font("\\data\\gfx\\fonts\\arial.ttf");
}

TEST_F(FontTests, whenCreatedWitEmptyFilePath_shouldThrowException)
{
    EXPECT_THROW((Font("")), he::common::invalid_initialization);
}

TEST_F(FontTests, whenCreatedWithWrongFilePathString_shouldThrowException)
{
    EXPECT_THROW((Font("data\\fonts\\wrongpath\\arial.ttf")), he::common::invalid_initialization);
}

TEST_F(FontTests, whenCreatedWithWrongFilePath_shouldThrowException)
{
    EXPECT_THROW((Font("\\data\\fonts\\wrongpath\\arial.ttf")), he::common::invalid_initialization);
}
} // namespace he::gfx::text