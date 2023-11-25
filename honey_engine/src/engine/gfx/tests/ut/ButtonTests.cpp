#include <gtest/gtest.h>
#include "gfx/draw/Button.hpp"
#include "graphic/TextureMock.hpp"

using namespace ::testing;

namespace he::gfx::draw
{
class ButtonTests : public testing::Test
{
public:
    ButtonTests() = default;
    ~ButtonTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::draw::Button>("button1", textureMock);
    }

    std::unique_ptr<he::gfx::draw::Button> sut;
    std::shared_ptr<he::gfx::render::TextureMock> textureMock = std::make_shared<he::gfx::render::TextureMock>();
};

TEST_F(ButtonTests, whenCreate_shouldCreateWithoutError)
{
}
} // namespace he::gfx::draw