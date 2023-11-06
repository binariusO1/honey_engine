#include <gtest/gtest.h>
#include "gfx/draw/Text.hpp"
#include "gfx/text/Font.hpp"
#include "gfx/render/Render.hpp"

namespace
{
} // namespace


namespace  he::gfx::draw
{
class TextTests : public testing::Test
{
public:
    TextTests() = default;
    ~TextTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::draw::Text>("text");
    }

    std::unique_ptr<he::gfx::draw::Text> sut;
};

TEST_F(TextTests, whenCreate_shouldCreateWithoutError)
{
    createSut();
}

TEST_F(TextTests, whenCreate_shouldSetFont)
{
    createSut();
    he::gfx::text::Font defaultFont;
    sut->setFont(defaultFont);
}
} // namespace he::gfx::draw