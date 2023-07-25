#include <libs/glad/glad.h> // note: it must be higher than other gl libraries
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>
#include <limits>
#include "exception/invalid_initialization.hpp"
#include "gfx/graphic/Texture.hpp"
#include "gl/GlWrapperMock.hpp"
#include "logger/Logger.hpp"

using namespace ::testing;
namespace
{
} // namespace
namespace  he::gfx::render
{
class TestedTexture: public Texture
{
public:
    TestedTexture(
        std::shared_ptr<he::libs::gl::IGlWrapper> ptr) 
    : Texture()
    {
        m_glWrapper = ptr;
    }
};

class TextureTestsFixture
{
public:
    void createSut()
    {
        sut = std::make_unique<he::gfx::render::TestedTexture>(glWrapperMock);
    }

    std::unique_ptr<he::gfx::render::TestedTexture> sut;
    std::shared_ptr<he::libs::gl::GlWrapperMock> glWrapperMock = std::make_shared<he::libs::gl::GlWrapperMock>();
};

class TextureTests : public testing::Test , public TextureTestsFixture
{
public:
    TextureTests() = default;
    ~TextureTests() override {};

    void expectedCreateTexture()
    {
        EXPECT_CALL(*glWrapperMock, generateTextureId(A<unsigned int&>())).Times(1).WillOnce(Invoke([&](unsigned int& value){ value = 1;}));
        EXPECT_CALL(*glWrapperMock, getIntegerv(A<unsigned int>(), A<int*>())).Times(1).WillOnce(Invoke([&](unsigned int, int* value){ *value = 4096;}));
        EXPECT_CALL(*glWrapperMock, bindTexture(A<unsigned int>(), A<unsigned int>())).Times(1);
        EXPECT_CALL(*glWrapperMock, specifyTextureImage2D(_, _, _, _, _, _, _, _, _)).Times(1);
        EXPECT_CALL(*glWrapperMock, setTextureParameters(_, _, _)).Times(4);
        EXPECT_CALL(*glWrapperMock, generateMipMap(_)).Times(1);
        EXPECT_CALL(*glWrapperMock, flush()).Times(1);
    }

    void expectedDelete()
    {
        EXPECT_CALL(*glWrapperMock, deleteTexture(_)).Times(1);
    }
};

TEST_F(TextureTests, whenCreate_shouldCreateWithoutError)
{
    const he::gfx::geometry::Size2Dpxl imageSize(255, 255);

    expectedCreateTexture();
    expectedDelete();

    createSut();
    sut->create(imageSize);
    ASSERT_EQ(sut->getSize(), imageSize);
}

TEST_F(TextureTests, whenCreateAndSizeIsNull_shouldReturnFalse)
{
    const he::gfx::geometry::Size2Dpxl imageSize(0, 100);
    EXPECT_CALL(*glWrapperMock, generateTextureId(_)).Times(0);
    createSut();
    ASSERT_EQ(sut->create(imageSize), false);
}
} // namespace he::gfx::render