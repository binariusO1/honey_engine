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
    MOCK_METHOD(bool, loadFromFile, (const std::filesystem::path&), (override));
    MOCK_METHOD(const float, getUnderlinePosition, (const unsigned int), (const override));
    MOCK_METHOD(const float, getUnderlineThickness, (const unsigned int), (const override));
    MOCK_METHOD(const Glyph&, getGlyph, (const std::uint32_t, const unsigned int, const bool, const float), (const override));
    MOCK_METHOD(float, getLineSpacing, (unsigned int), (const override));
    MOCK_METHOD(float, getKerning, (std::uint32_t, std::uint32_t, unsigned int, bool), (const override));

    MOCK_METHOD(he::gfx::text::Page&, loadPage, (const unsigned int), (const override));
};
} // namespace text
} // namespace gfx
} // namespace he