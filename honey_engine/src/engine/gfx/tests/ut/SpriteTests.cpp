#include <gtest/gtest.h>
#include "gfx/draw/Sprite.hpp"
#include "graphic/TextureMock.hpp"

using namespace ::testing;
namespace
{
} // namespace
namespace  he::gfx::draw
{
class SpriteTests : public testing::Test
{
public:
    SpriteTests() = default;
    ~SpriteTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::draw::Sprite>("sprite", textureMock);
    }

    std::unique_ptr<he::gfx::draw::Sprite> sut;
    std::shared_ptr<he::gfx::render::TextureMock> textureMock = std::make_shared<he::gfx::render::TextureMock>();
};

TEST_F(SpriteTests, whenCreate_shouldCreateWithoutError)
{
    const he::gfx::geometry::Size2Dpxl size(200, 200);
    EXPECT_CALL(*textureMock, getSize()).Times(2).WillRepeatedly(Return(size));
    createSut();
}
} // namespace he::gfx::draw