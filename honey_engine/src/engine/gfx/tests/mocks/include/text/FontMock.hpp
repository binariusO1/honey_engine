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

    MOCK_METHOD(const std::shared_ptr<he::gfx::render::Texture>, getTexture, (), (const override));
    MOCK_METHOD(bool, loadFromFile, (const std::filesystem::path&), (override));
    MOCK_METHOD(const float, getUnderlinePosition, (), (const override));
    MOCK_METHOD(const float, getUnderlineThickness, (), (const override));
    MOCK_METHOD(const Glyph&, getGlyph, (const std::uint32_t, const bool, const float), (const override));
    MOCK_METHOD(float, getLineSpacing, (), (const override));
    MOCK_METHOD(float, getKerning, (std::uint32_t, std::uint32_t, bool), (const override));
    MOCK_METHOD(bool, setCharacterSize, (const unsigned int), (override));
    MOCK_METHOD(const unsigned int, getCharacterSize, (), (const override));

    MOCK_METHOD(he::gfx::text::Page&, loadPage, (), (const override));
};
} // namespace text
} // namespace gfx
} // namespace he