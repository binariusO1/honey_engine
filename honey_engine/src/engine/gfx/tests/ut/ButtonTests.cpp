#include <gtest/gtest.h>
#include "gfx/draw/Button.hpp"
#include "graphic/TextureMock.hpp"
#include "window/events/Event.hpp"

using namespace ::testing;

namespace
{
constexpr he::gfx::geometry::Size2Dpxl t_buttonSize{100, 100};
} // namespace

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

    void expectInitCalls()
    {
        EXPECT_CALL(*textureMock, getSize()).WillRepeatedly(Return(t_buttonSize));
    }

    std::unique_ptr<he::gfx::draw::Button> sut;
    std::shared_ptr<he::gfx::render::TextureMock> textureMock = std::make_shared<he::gfx::render::TextureMock>();
};

TEST_F(ButtonTests, construction_whenCreate_shouldCreateWithoutError)
{
    expectInitCalls();

    createSut();
}

TEST_F(ButtonTests, onMauseButtonPressed_whenButtonPressed_shouldReturnTrue)
{
    expectInitCalls();
    he::window::Event::MouseButtonAction event{he::window::Mouse::Button::Left, 1, 99};

    createSut();
    
    ASSERT_EQ(sut->onMauseButtonPressed(event), true);
}

TEST_F(ButtonTests, onMauseButtonPressed_whenButtonPressed_shouldReturnFalse)
{
    expectInitCalls();
    he::window::Event::MouseButtonAction event{he::window::Mouse::Button::Left, 101, 101};

    createSut();
    
    ASSERT_EQ(sut->onMauseButtonPressed(event), false);
}
} // namespace he::gfx::draw