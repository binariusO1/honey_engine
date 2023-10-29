#include "gfx/render/SceneManager.hpp"

#include "exception/invalid_initialization.hpp"
#include "logger/Logger.hpp"

namespace
{
} // namespace

namespace he
{
namespace gfx
{
namespace render
{
////////////////////////////////////////////////////////////
SceneManager::SceneManager(const SceneTransitionTable& transitionTable) : m_stateMachine{std::make_unique<he::gfx::render::OpenCloseStateMachine>()}
{
    if (transitionTable.empty())
    {
        throw he::common::invalid_initialization("Invalid initalization. Scenes list is empty!");
    }
    addTransitionTable(transitionTable);
    m_currentScene->open();
    LOG_DEBUG << "Current scene is: " << m_currentScene->getName();
}


////////////////////////////////////////////////////////////
SceneManager::~SceneManager()
{
}


////////////////////////////////////////////////////////////
void SceneManager::render(gfx::render::IRender& render)
{
    if (m_currentScene->isOpen())
    {
        m_currentScene->render(render);
    }
    else
    {
        for (auto& scene : m_scenes)
        {
            if (scene.second->isOpen())
            {
                setCurrentScene(scene.second);
                m_currentScene->render(render);
                LOG_DEBUG << "Current scene is now: " << m_currentScene->getName();
                return;
            }
        }
        throw he::common::invalid_initialization("Logic error. All scenes are closed!");
    }
}


////////////////////////////////////////////////////////////
void SceneManager::addTransitionTable(const SceneTransitionTable& transitionTable)
{
    for (auto& transition : transitionTable)
    {
        if (not isSceneValid(transition.fromState))
        {
            throw he::common::invalid_initialization("Unable to add transition table!");
            return;
        }
        m_stateMachine->addTransition(transition);

        if(m_scenes.empty() or m_currentScene == nullptr)
        {
            m_currentScene = transition.fromState.get();
            m_stateMachine->setCurrentState(m_currentScene);
        }
        auto result = m_scenes.insert({transition.fromState->getName(), transition.fromState});
        if (not result.second)
        {
            throw he::common::invalid_initialization("Unable to add transition table! Key is already exists");
            return;
        }
        if (transition.toState != nullptr and transition.toState.get() != nullptr and not transition.toState->getName().empty())
        {
            auto result = m_scenes.insert({transition.toState->getName(), transition.toState});
            if (not result.second)
            {
                throw he::common::invalid_initialization("Unable to add transition table! Key is already exists");
                return;
            }
        }
    }
}


////////////////////////////////////////////////////////////
void SceneManager::process_event(const he::window::Event& event)
{
    m_stateMachine->process_event<he::window::Event>(event);
    if (m_currentScene->isOpen())
    {
        m_currentScene->process_event(event);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRIVATE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
void SceneManager::setCurrentScene(const std::shared_ptr<IScene>& scene)
{
    m_currentScene = scene.get();
}

////////////////////////////////////////////////////////////
bool SceneManager::isSceneValid(const std::shared_ptr<IScene>& scene)
{
    if(scene == nullptr)
    { 
        LOG_ERROR << "Pointer is not initialized.";
        return false;
    }
    if (scene->getName().empty())
    {
        LOG_ERROR << "Scene has no name!";
        return false;
    }
    return true;
}
} // namespace render
} // namespace gfx
} // namespace he