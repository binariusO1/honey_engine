#pragma once

#include <gmock/gmock-function-mocker.h>
#include "libs/gl/IGlWrapper.hpp"

namespace he
{
namespace libs
{
namespace gl
{
class GlWrapperMock : public IGlWrapper
{
public:
    GlWrapperMock() = default;
    ~GlWrapperMock() override = default;

    MOCK_METHOD(void, setupDraw, (const GLsizeiptr, std::size_t, const void*, const DrawType), (override));
    MOCK_METHOD(void, draw, (std::size_t, std::size_t, const uint8_t*, const int, he::libs::gl::ConnectionType, int, bool), (override));
    MOCK_METHOD(void, drawPrimitive, (std::size_t, std::size_t, const uint8_t*, const int, he::libs::gl::ConnectionType, int, bool), (override));

    MOCK_METHOD(void, getIntegerv, (const unsigned int, int*), (const override));
    MOCK_METHOD(void, bindTexture, (const unsigned int, const unsigned int), (const override));
    MOCK_METHOD(void, flush, (), (const override));
    MOCK_METHOD(void, generateTextureId, (unsigned int&), (const override));
    MOCK_METHOD(void, bindTexture, (unsigned int&), (const override));
    MOCK_METHOD(void, specifyTextureImage2D, (const unsigned int target, 
                                        const int level, 
                                        const int internalformat,
                                        const unsigned int width, 
                                        const unsigned int height, 
                                        const int border, 
                                        const unsigned int format, 
                                        const unsigned int type, 
                                        const void * data), (const override));
    MOCK_METHOD(void, specifyTextureSubImage2D, (const unsigned int target, 
                                        const int level, 
                                        const int xoffset, 
                                        const int yoffset, 
                                        const int width, 
                                        const int height, 
                                        const unsigned int format, 
                                        const unsigned int type, 
                                        const void * pixels), (const override));
    MOCK_METHOD(void, setTextureParameters, (const unsigned int, const unsigned int, const int), (const override));
    MOCK_METHOD(void, generateMipMap, (const unsigned int), (const override));
    MOCK_METHOD(void, deleteTexture, (const unsigned int), (const override));
    MOCK_METHOD(void, clear, (const int&), (const override));
};
} // namespace gl
} // namespace libs
} // namespace he