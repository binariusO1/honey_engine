#pragma once

#include "gfx/draw/Text.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
class TextMock : public Text
{
public:
    ~TextMock() override {};

public:
    TextMock(std::string name, std::shared_ptr<he::gfx::text::IFont> ptr) : Text(name)
    {
        m_fontMock = ptr;
        m_font = ptr;
    }

    void createNewFont(const text::Font& font) override
    {
        m_font = m_fontMock;
        (void)font;
    }

private:
    std::shared_ptr<he::gfx::text::IFont> m_fontMock;
};
} // namespace render
} // namespace gfx
} // namespace he