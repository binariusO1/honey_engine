#include <gtest/gtest.h>
#include "gfx/render/Button.hpp"
#include "logger/Logger.hpp"

using namespace ::testing;

namespace he::gfx::render
{
class ButtonTests : public testing::Test
{
public:
    ButtonTests() = default;
    ~ButtonTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::render::Button>("button1");
    }

    std::unique_ptr<he::gfx::render::Button> sut;
};

TEST_F(ButtonTests, whenCreate_shouldCreateWithoutError)
{
}
} // namespace he::gfx::render