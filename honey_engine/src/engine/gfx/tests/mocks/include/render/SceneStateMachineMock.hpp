#pragma once

#include <gmock/gmock-function-mocker.h>
#include "gfx/render/IStateMachine.hpp"

namespace he
{
namespace gfx
{
namespace render
{
class IScene;

class OpenCloseStateMachineMock : public IStateMachine<he::gfx::render::IScene>
{
public:
    OpenCloseStateMachineMock() = default;
    ~OpenCloseStateMachineMock() override = default;
    MOCK_METHOD(void, setCurrentState, (IScene*), (override));
    MOCK_METHOD(void, addTransition, (std::shared_ptr<IScene> fromState, 
                        std::shared_ptr<IEvent> event, 
                        std::shared_ptr<IScene> toState,
                        std::shared_ptr<IAction> action,
                        std::shared_ptr<IGuard> guard), (override));
    MOCK_METHOD(void, addTransition, (const Transition<IScene>&), (override));
    MOCK_METHOD(void, process_event, (const std::shared_ptr<IEvent>&), (override));
};
} // namespace render
} // namespace gfx
} // namespace he