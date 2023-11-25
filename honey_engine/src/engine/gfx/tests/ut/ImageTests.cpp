#include <gtest/gtest.h>
#include "exception/invalid_initialization.hpp"
#include "gfx/Color.hpp"
#include "gfx/geometry/Size2D.hpp"
#include "gfx/graphic/Image.hpp"

using namespace ::testing;
namespace
{
const he::gfx::Color color(he::gfx::Color::White);
} // namespace
namespace  he::gfx::render
{
class ImageTests : public testing::Test
{
public:
    ImageTests() = default;
    ~ImageTests() override {};

    void createSut()
    {
        const geometry::Size2Dpxl size({2, 2});

        sut = std::make_unique<he::gfx::render::Image>(size, color);
    }
    std::unique_ptr<he::gfx::render::Image> sut;
};

TEST_F(ImageTests, whenCreateAndSizeIsNull_shouldThrowException)
{
    const he::gfx::geometry::Size2Dpxl imageSize(0, 10);
    EXPECT_THROW((he::gfx::render::Image(imageSize, color)), he::common::invalid_initialization);
}

} // namespace he::gfx::render