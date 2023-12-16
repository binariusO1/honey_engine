#pragma once

#include <gmock/gmock-function-mocker.h>
#include "gfx/render/IRender.hpp"

namespace he
{
namespace gfx
{
namespace render
{
class RenderMock : public IRender
{
public:
    RenderMock() = default;
    ~RenderMock() override = default;

    MOCK_METHOD(void, draw, (he::gfx::draw::IDrawable&, TransformMatrix&), (override));
    MOCK_METHOD(void, draw, (he::gfx::draw::IDrawable&, const he::gfx::render::RenderSettings&, TransformMatrix&), (override));
};
} // namespace render
} // namespace gfx
} // namespace he