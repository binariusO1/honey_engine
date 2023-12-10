#include <gtest/gtest.h>
#include "gfx/render/UniquePropagationLayer.hpp"
#include "graphic/TextureMock.hpp"

using namespace ::testing;
namespace
{
} // namespace
namespace  he::gfx::render
{
class UniquePropagationLayerTests : public testing::Test
{
public:
    UniquePropagationLayerTests() = default;
    ~UniquePropagationLayerTests() override {};

    void createSut(const PropagationSettings& propagationSettings)
    {
        sut = std::make_unique<he::gfx::render::UniquePropagationLayer>("unique propagation layer", propagationSettings);
    }

    std::unique_ptr<he::gfx::render::UniquePropagationLayer> sut;
    std::shared_ptr<he::gfx::render::TextureMock> textureMock = std::make_shared<he::gfx::render::TextureMock>();
    std::shared_ptr<he::gfx::render::ITexture> textureBase = textureMock;
    std::shared_ptr<he::gfx::render::Texture> texture = std::static_pointer_cast<he::gfx::render::Texture>(textureBase);
};

TEST_F(UniquePropagationLayerTests, addButton_whenAddButtonAndPropagate_shouldSetOrder)
{
    // todo
}

} // namespace he::gfx::render