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
        sut = std::make_unique<he::gfx::render::Layer2d>("test name");
    }

    std::unique_ptr<he::gfx::render::Layer2d> sut;
};

TEST_F(LayerTests, whenCreateWithEmptyName_shouldThrowException)
{
    EXPECT_THROW((he::gfx::render::Layer2d("")), he::common::invalid_initialization);
}

TEST_F(LayerTests, whenaddDrawableWithSameName_shouldAddOnlyFirst)
{
    std::shared_ptr<he::gfx::geometry::figures::Figure> hex = std::make_shared<he::gfx::geometry::figures::Hexagon>(55.0);
    std::shared_ptr<he::gfx::draw::Shape2d> hexagon6 = std::make_shared<he::gfx::draw::Shape2d>("hex6", *hex);
    hexagon6->setColor(he::gfx::Color::Magenta);
    hexagon6->setPosition({200.0, 500.0});
    std::shared_ptr<he::gfx::draw::Shape2d> hexagon7 = std::make_shared<he::gfx::draw::Shape2d>("hex6", *hex);
    hexagon7->setColor(he::gfx::Color::Red);
    hexagon7->setPosition({200.0, 600.0});

    std::shared_ptr<he::gfx::render::Layer2d> layer2 = std::make_shared<he::gfx::render::Layer2d>("layer_2");
    he::gfx::render::ShapeList hexagons{hexagon6, hexagon7};
    layer2->addShapes(hexagons);

    ASSERT_EQ(layer2->getShapeList().size(), 1);
}
} // namespace he::gfx::render