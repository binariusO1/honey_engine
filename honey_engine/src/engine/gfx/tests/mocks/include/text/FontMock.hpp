#pragma once

#include <gmock/gmock-function-mocker.h>
#include "gfx/text/IFont.hpp"

namespace he
{
namespace gfx
{
namespace text
{
class FontMock : public IFont
{ 
public:
    FontMock() = default;
    ~FontMock() override = default;

    MOCK_METHOD(const std::shared_ptr<he::gfx::render::Texture>, getTexture, (const unsigned int), (const override));
    MOCK_METHOD(he::gfx::text::Page&, loadPage, (const unsigned int), (const override));
};
} // namespace text
} // namespace gfx
} // namespace he