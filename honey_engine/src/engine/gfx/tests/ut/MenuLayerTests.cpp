#include <gtest/gtest.h>
#include "gfx/render/MenuLayer.hpp"
#include "graphic/TextureMock.hpp"

using namespace ::testing;
namespace
{
} // namespace
namespace  he::gfx::render
{
class MenuLayerTests : public testing::Test
{
public:
    MenuLayerTests() = default;
    ~MenuLayerTests() override {};

    void createSut(const PropagationSettings& propagationSettings)
    {
        sut = std::make_unique<he::gfx::render::MenuLayer>("menu layer", propagationSettings);
    }

    std::unique_ptr<he::gfx::render::MenuLayer> sut;
    std::shared_ptr<he::gfx::render::TextureMock> textureMock = std::make_shared<he::gfx::render::TextureMock>();
    std::shared_ptr<he::gfx::render::ITexture> textureBase = textureMock;
    std::shared_ptr<he::gfx::render::Texture> texture = std::static_pointer_cast<he::gfx::render::Texture>(textureBase);
};

TEST_F(MenuLayerTests, setButton_whenSetButtonSecondTime_shouldOverrideExistedButton)
{
    EXPECT_CALL(*textureMock, getSize())
        .WillOnce(Return(geometry::Size2Dpxl{160, 40})).WillOnce(Return(geometry::Size2Dpxl{160, 40}))
        .WillOnce(Return(geometry::Size2Dpxl{200, 50})).WillOnce(Return(geometry::Size2Dpxl{200, 50}));
    createSut({1, 5, 0, 20});
    he::gfx::draw::Button button1("Button1", textureMock);
    he::gfx::draw::Button button2("Button2", textureMock);

    sut->setButton(std::make_shared<he::gfx::draw::Button>(button1));
    sut->setButton(std::make_shared<he::gfx::draw::Button>(button2));

    ASSERT_EQ(sut->getButtons().size(), 5);
    ASSERT_EQ(sut->getButtons()[0]->getSize().width, 200.f);
    ASSERT_EQ(sut->getButtons()[0]->getSize().height, 50.f);
}

} // namespace he::gfx::render