#pragma once

#include <functional>
#include <string>
#include "gfx/render/IStateMachine.hpp"
#include "gfx/render/IScene.hpp"
#include "logger/Logger.hpp"
#include "window/events/Event.hpp"
#include "window/events/IEventInputListener.hpp"

namespace he
{
namespace gfx
{
namespace render
{
class IScene;

class OpenCloseStateMachine : public he::IStateMachine<IScene> , public he::window::IEventInputListener
{
public:
    OpenCloseStateMachine() = default;
    ~OpenCloseStateMachine() override {};

    void setCurrentState(IScene* state) override {
        m_currentState = state;
    }

    void addTransition( std::shared_ptr<IScene> fromState, 
                        std::shared_ptr<IEvent> event, 
                        std::shared_ptr<IScene> toState,
                        std::shared_ptr<IAction> action,
                        std::shared_ptr<IGuard> guard) override 
    {
        m_transitions.push_back({fromState, event, toState, action, [guard]() { return guard->check(); }});
    }

    void addTransition(const Transition<IScene>& transition) override
    {
        m_transitions.push_back(transition);
    }

    template<typename E>
    void process_event(const E event)
    {
        if (m_currentState == nullptr)
        {
            LOG_ERROR << "Cannot make transition. Current scene or start scene is nullptr!";
            return; 
        }
        for (const auto& transition : m_transitions) 
        {
            if (transition.fromState == nullptr)
            {
                LOG_WARNING << "Cannot make transition. Current scene or start scene is nullptr!";
                return;
            }
            if (transition.event != nullptr)
            {
                std::shared_ptr<he::Event<E>> derivedPtr = std::dynamic_pointer_cast<he::Event<E>>(transition.event);
                if (m_currentState == transition.fromState.get() and event == derivedPtr->data)
                {
                    if (transition.action != nullptr)
                    {
                        transition.action->execute();
                    }
                    if (transition.toState != nullptr and m_currentState != transition.toState.get())
                    {
                        transition.toState->open();
                        transition.fromState->close();
                        LOG_INFO << "Scene transition from: " << transition.fromState->getName() << " to: " << transition.toState->getName();
                        break;
                    }
                }
            }
        }
    }

private:
    void process_event(const std::shared_ptr<IEvent>& event) override 
    {
        if (m_currentState == nullptr)
        {
            LOG_ERROR << "Cannot make transition. Current scene or start scene is nullptr!";
            return; 
        }
        for (const auto& transition : m_transitions) 
        {
            if (transition.fromState == nullptr)
            {
                LOG_WARNING << "Cannot make transition. Current scene or start scene is nullptr!";
                return;
            }
            if (event != nullptr and transition.event != nullptr)
            {
                if (m_currentState == transition.fromState.get() and typeid(event.get()) == typeid(transition.event.get()))
                {
                    if (transition.action != nullptr)
                    {
                        transition.action->execute();
                    }
                    if (transition.toState != nullptr and m_currentState != transition.toState.get())
                    {
                        transition.fromState->close();
                        transition.toState->open();
                        LOG_DEBUG << "Scene transition from: " << transition.fromState->getName() << " to: " << transition.toState->getName();
                    }
                    break;
                }
            }
        }
    }    
    
public:
    void process_event(const he::window::Event&) override
    {
    }

private:
    IScene* m_currentState;
    std::vector<typename he::Transition<IScene>> m_transitions;
};
} // namespace render
} // namespace gfx
} // namespace he