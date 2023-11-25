
#include <gtest/gtest.h>
#include "gfx/render/Scene.hpp"
#include "gfx/render/Layer.hpp"
#include "render/LayerMock.hpp"
#include "render/RenderMock.hpp"

using namespace ::testing;
namespace
{
} // namespace
namespace  he::gfx::render
{
class SceneTests : public testing::Test
{
public:
    SceneTests() = default;
    ~SceneTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::render::Scene>("test name");
    }

    std::unique_ptr<he::gfx::render::Scene> sut;
    he::gfx::render::RenderMock renderMock;
};

TEST_F(SceneTests, whenCreateWithEmptyName_shouldThrowException)
{
    EXPECT_THROW((he::gfx::render::Scene("")), he::common::invalid_initialization);
}

TEST_F(SceneTests, shouldAddAndRemoveLayers)
{
    std::shared_ptr<he::gfx::render::Layer> layer1 = std::make_shared<he::gfx::render::Layer>("layer1");
    std::shared_ptr<he::gfx::render::Layer> layer2 = std::make_shared<he::gfx::render::Layer>("layer2");
    std::shared_ptr<he::gfx::render::Layer> layer3 = std::make_shared<he::gfx::render::Layer>("layer3");

    createSut();
    sut->addLayers({layer1, layer2});
    sut->addLayer(layer3);
    sut->removeLayer(layer1);
    sut->removeLayer(layer2);
    sut->removeLayer(layer3);
}

TEST_F(SceneTests, shouldRenderOnlyVisibleLayers)
{
    std::shared_ptr<he::gfx::render::LayerMock> LayerMock1 = std::make_shared<he::gfx::render::LayerMock>();
    std::shared_ptr<he::gfx::render::LayerMock> LayerMock2 = std::make_shared<he::gfx::render::LayerMock>();

    EXPECT_CALL(*LayerMock1, hide());
    EXPECT_CALL(*LayerMock1, isVisible()).WillOnce(Return(false));
    EXPECT_CALL(*LayerMock2, isVisible()).WillOnce(Return(true));
    EXPECT_CALL(*LayerMock2, render(_)).Times(1);
    EXPECT_CALL(*LayerMock1, getName()).WillRepeatedly(Return("layer1"));
    EXPECT_CALL(*LayerMock2, getName()).WillRepeatedly(Return("layer2"));

    createSut();
    sut->addLayers({LayerMock1, LayerMock2});
    LayerMock1->hide();
    sut->render(renderMock);
}

} // namespace he::gfx::render