#pragma once

#include <gmock/gmock-function-mocker.h>
#include "gfx/graphic/Texture.hpp"

namespace he
{
namespace gfx
{
namespace render
{
class TextureMock : public ITexture
{
public:
    TextureMock() = default;
    ~TextureMock() override = default;

    MOCK_METHOD(bool, create, (const geometry::Size2Dpxl&, const std::uint8_t*), (override));
    MOCK_METHOD(bool, createEmpty, (const he::gfx::geometry::Size2Dpxl&), (override));
    MOCK_METHOD(void, setSmooth, (bool), (override));
    MOCK_METHOD(const he::gfx::geometry::Size2Dpxl, getSize, (), (const override));
    MOCK_METHOD(const unsigned int, getTextureId, (), (const override));
};
} // namespace render
} // namespace gfx
} // namespace he