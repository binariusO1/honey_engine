#pragma once

#include <gmock/gmock-function-mocker.h>
#include "gfx/render/IScene.hpp"

namespace he
{
namespace gfx
{
namespace render
{
class IRender;

class SceneMock : public IScene
{
public:
    SceneMock() = default;
    ~SceneMock() override = default;

    MOCK_METHOD(void, open, (), (override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(bool, isOpen, (), (const override));
    MOCK_METHOD(void, render, (gfx::render::IRender&), (override));
    MOCK_METHOD(const std::string, getName, (), (override));
};
} // namespace render
} // namespace gfx
} // namespace he