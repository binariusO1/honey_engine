#pragma once

#include <map>
#include <memory>
#include <vector>
#include "gfx/event/Event.hpp"
#include "gfx/render/IRenderable.hpp"
#include "gfx/render/IStateMachine.hpp"
#include "gfx/render/Scene.hpp"
#include "gfx/render/SceneStateMachine.hpp"
#include "window/events/IEventInputListener.hpp"

namespace he
{
namespace gfx
{
namespace render
{
using ScenesMap = std::map<const std::string, std::shared_ptr<IScene>>;
using SceneTransitionTable = std::vector<he::Transition<IScene>>;
class SceneManager : public IRenderable , public he::window::IEventInputListener
{
public:
    SceneManager(const SceneTransitionTable&);
    ~SceneManager() override;

public:
    void render(gfx::render::IRender&) override;

public:
    void addTransitionTable(const SceneTransitionTable& transitions);

public:
    void process_event(const he::window::Event&);

private:
    template <typename E>
    void addTransition(     const std::shared_ptr<he::gfx::render::IScene>& startState, 
                            E event,
                            const std::shared_ptr<he::gfx::render::IScene>& endState, 
                            std::shared_ptr<IAction> action, 
                            std::shared_ptr<IGuard> guard)
    {
        if (not isSceneValid(startState))
        {
            return;
        }
        std::shared_ptr<he::Event<E>> convertedEvent = std::make_shared<he::Event<E>>(event);
        m_stateMachine->addTransition(startState.get(), convertedEvent, endState.get(), action, guard);

        if(m_scenes.empty() and m_currentScene == nullptr)
        {
            m_currentScene = startState.get();
            m_stateMachine->setCurrentState(m_currentScene);
        }
        m_scenes.insert({startState->getName(), std::move(startState)});
        if (isSceneValid(endState))
        {
            m_scenes.insert({endState->getName(), std::move(endState)});
        }
    }

private:
    void setCurrentScene(const std::shared_ptr<IScene>&);
    bool isSceneValid(const std::shared_ptr<IScene>&);

protected:
    ScenesMap m_scenes{};
    IScene* m_currentScene{nullptr};
    std::unique_ptr<he::gfx::render::OpenCloseStateMachine> m_stateMachine{nullptr};
};
} // namespace render
} // namespace gfx
} // namespace he