#include <gtest/gtest.h>
#include "gfx/render/UniquePropagationLayer.hpp"
#include "graphic/TextureMock.hpp"

using namespace ::testing;
namespace
{
const he::gfx::geometry::Size2Dpxl t_buttonSize{160, 40};
const he::gfx::geometry::Point2Df t_buttonPos{100, 100};
} // namespace
namespace  he::gfx::render
{

class UniquePropagationLayerTestsFixture
{
public:
    void createSut(const PropagationSettings& propagationSettings)
    {
        sut = std::make_unique<he::gfx::render::UniquePropagationLayer>("unique propagation layer", propagationSettings);
    }

    std::unique_ptr<he::gfx::render::UniquePropagationLayer> sut;
    std::shared_ptr<he::gfx::render::TextureMock> textureMock = std::make_shared<he::gfx::render::TextureMock>();
    std::shared_ptr<he::gfx::render::ITexture> textureBase = textureMock;
    std::shared_ptr<he::gfx::render::Texture> texture = std::static_pointer_cast<he::gfx::render::Texture>(textureBase);
};

class UniquePropagationLayerTests : public testing::Test , public UniquePropagationLayerTestsFixture
{
public:
    UniquePropagationLayerTests() = default;
    ~UniquePropagationLayerTests() override {};
};
class UniquePropagationLayerOrderTests : public testing::TestWithParam<std::tuple<gfx::PropagationSettings, geometry::Point2Df, geometry::Point2Df, geometry::Point2Df>> , public UniquePropagationLayerTestsFixture
{
};

TEST_P(UniquePropagationLayerOrderTests, addButton_whenAddButtonAndPropagate_shouldSetOrderAxis)
{
    const auto [p_distance, p_expectedPosButton2, p_expectedPosButton3, p_expectedPosButton4] = GetParam();

    EXPECT_CALL(*textureMock, getSize()).WillOnce(Return(t_buttonSize)).WillOnce(Return(t_buttonSize));
    he::gfx::draw::Button button1("Button1", textureMock);
    button1.setPosition(t_buttonPos);

    createSut(p_distance);
    sut->addButton(std::make_shared<he::gfx::draw::Button>(button1));

    auto buttons = sut->getButtons();
    ASSERT_EQ(buttons[0]->getPosition(), t_buttonPos);
    ASSERT_EQ(buttons[1]->getPosition(), p_expectedPosButton2);
    ASSERT_EQ(buttons[2]->getPosition(), p_expectedPosButton3);
    ASSERT_EQ(buttons[3]->getPosition(), p_expectedPosButton4);
}

TEST_P(UniquePropagationLayerOrderTests, addButton_whenAddButtonWithSetOriginInCenterAndPropagate_shouldSetOrderAxis)
{
    const auto [p_distance, p_expectedPosButton2, p_expectedPosButton3, p_expectedPosButton4] = GetParam();

    EXPECT_CALL(*textureMock, getSize()).WillOnce(Return(t_buttonSize)).WillOnce(Return(t_buttonSize));
    he::gfx::draw::Button button1("Button1", textureMock);
    button1.setPosition(t_buttonPos);
    button1.setOriginInCenter();

    createSut(p_distance);
    sut->addButton(std::make_shared<he::gfx::draw::Button>(button1));

    auto buttons = sut->getButtons();
    ASSERT_EQ(buttons[0]->getPosition(), t_buttonPos);
    ASSERT_EQ(buttons[1]->getPosition(), p_expectedPosButton2);
    ASSERT_EQ(buttons[2]->getPosition(), p_expectedPosButton3);
    ASSERT_EQ(buttons[3]->getPosition(), p_expectedPosButton4);
}

INSTANTIATE_TEST_SUITE_P(
    UniquePropagationLayerTests, 
    UniquePropagationLayerOrderTests,
    testing::Values(
        std::make_tuple(
            gfx::PropagationSettings{1, 4, 0, 100}, 
            geometry::Point2Df{t_buttonPos.x, t_buttonPos.y + t_buttonSize.height + 100}, 
            geometry::Point2Df{t_buttonPos.x, t_buttonPos.y + t_buttonSize.height * 2 + 100 * 2},
            geometry::Point2Df{t_buttonPos.x, t_buttonPos.y + t_buttonSize.height * 3 + 100 * 3}),
        std::make_tuple(
            gfx::PropagationSettings{1, 4, 0, -100}, 
            geometry::Point2Df{t_buttonPos.x, t_buttonPos.y - t_buttonSize.height - 100}, 
            geometry::Point2Df{t_buttonPos.x, t_buttonPos.y - t_buttonSize.height * 2 - 100 * 2},
            geometry::Point2Df{t_buttonPos.x, t_buttonPos.y - t_buttonSize.height * 3 - 100 * 3}),
        std::make_tuple(
            gfx::PropagationSettings{4, 1, 100, 0}, 
            geometry::Point2Df{t_buttonPos.x + t_buttonSize.width + 100, t_buttonPos.y}, 
            geometry::Point2Df{t_buttonPos.x + t_buttonSize.width * 2 + 100 * 2, t_buttonPos.y},
            geometry::Point2Df{t_buttonPos.x + t_buttonSize.width * 3 + 100 * 3, t_buttonPos.y}),
        std::make_tuple(
            gfx::PropagationSettings{4, 1, -100, 0}, 
            geometry::Point2Df{t_buttonPos.x - t_buttonSize.width - 100, t_buttonPos.y}, 
            geometry::Point2Df{t_buttonPos.x - t_buttonSize.width * 2 - 100 * 2, t_buttonPos.y},
            geometry::Point2Df{t_buttonPos.x - t_buttonSize.width * 3 - 100 * 3, t_buttonPos.y}),
        std::make_tuple(
            gfx::PropagationSettings{2, 2, 0, 0}, 
            geometry::Point2Df{t_buttonPos.x, t_buttonPos.y + t_buttonSize.height}, 
            geometry::Point2Df{t_buttonPos.x + t_buttonSize.width, t_buttonPos.y},
            geometry::Point2Df{t_buttonPos.x + t_buttonSize.width, t_buttonPos.y + t_buttonSize.height}),
        std::make_tuple(
            gfx::PropagationSettings{2, 2, 100, 100}, 
            geometry::Point2Df{t_buttonPos.x, t_buttonPos.y + t_buttonSize.height + 100}, 
            geometry::Point2Df{t_buttonPos.x + t_buttonSize.width + 100, t_buttonPos.y},
            geometry::Point2Df{t_buttonPos.x + t_buttonSize.width + 100, t_buttonPos.y + t_buttonSize.height + 100}),
        std::make_tuple(
            gfx::PropagationSettings{2, 2, -100, -100}, 
            geometry::Point2Df{t_buttonPos.x, t_buttonPos.y - t_buttonSize.height - 100}, 
            geometry::Point2Df{t_buttonPos.x - t_buttonSize.width - 100, t_buttonPos.y},
            geometry::Point2Df{t_buttonPos.x - t_buttonSize.width - 100, t_buttonPos.y - t_buttonSize.height - 100})
        ));
} // namespace he::gfx::render