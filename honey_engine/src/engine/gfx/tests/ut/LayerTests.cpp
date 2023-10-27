
// #include <libs/glad/glad.h> // note: it must be higher than other gl libraries
// #include <GLFW/glfw3.h>
#include <gtest/gtest.h>
#include <memory>
#include "gfx/render/Layer.hpp"
#include "gfx/geometry/figures/Hexagon.hpp"
#include "gfx/draw/Shape.hpp"

using namespace ::testing;
namespace
{
} // namespace
namespace  he::gfx::render
{
class LayerTests : public testing::Test
{
public:
    LayerTests() = default;
    ~LayerTests() override {};

    void createSut()
    {
        sut = std::make_unique<he::gfx::render::Layer>("test name");
    }

    std::unique_ptr<he::gfx::render::Layer> sut;
};

TEST_F(LayerTests, whenCreateWithEmptyName_shouldThrowException)
{
    EXPECT_THROW((he::gfx::render::Layer("")), he::common::invalid_initialization);
}

TEST_F(LayerTests, whenaddDrawableWithSameName_shouldAddOnlyFirst)
{
    std::shared_ptr<he::gfx::geometry::figures::Figure> hex = std::make_shared<he::gfx::geometry::figures::Hexagon>(55.0);
    std::shared_ptr<he::gfx::draw::IDrawable> hexagon6 = std::make_shared<he::gfx::draw::Shape>("hex6", hex);
    hexagon6->setColor(he::gfx::Color::Magenta);
    hexagon6->setPosition({200.0, 500.0});
    std::shared_ptr<he::gfx::draw::IDrawable> hexagon7 = std::make_shared<he::gfx::draw::Shape>("hex6", hex);
    hexagon7->setColor(he::gfx::Color::Red);
    hexagon7->setPosition({200.0, 600.0});

    std::shared_ptr<he::gfx::render::Layer> layer2 = std::make_shared<he::gfx::render::Layer>("layer_2");
    he::gfx::render::DrawableList hexagons{hexagon6, hexagon7};
    layer2->addDrawables(hexagons);

    ASSERT_EQ(layer2->drawable("hex6").getColor(), he::gfx::Color::Magenta);
}
} // namespace he::gfx::render