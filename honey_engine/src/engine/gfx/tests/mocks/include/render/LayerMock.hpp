#pragma once

#include <gmock/gmock-function-mocker.h>
#include "gfx/render/ILayer.hpp"

namespace he
{
namespace gfx
{
namespace render
{
class IRender;

class LayerMock : public ILayer
{
public:
    LayerMock() = default;
    ~LayerMock() override = default;

    MOCK_METHOD(void, show, (), (override));
    MOCK_METHOD(void, hide, (), (override));
    MOCK_METHOD(bool, isVisible, (), (const override));
    MOCK_METHOD(void, render, (gfx::render::IRender&), (override));
    MOCK_METHOD(void, setRenderSettings, (const he::gfx::render::RenderSettings&), (override));
    MOCK_METHOD(void, process_event, (const he::window::Event&), (override));
    MOCK_METHOD(const std::string, getName, (), (const));
};
} // namespace render
} // namespace gfx
} // namespace he