
#include <gtest/gtest.h>
#include <memory>
#include "exception/invalid_initialization.hpp"
#include "gfx/render/IStateMachine.hpp"
#include "gfx/render/SceneStateMachine.hpp"
#include "gfx/render/Scene.hpp"
#include "render/RenderMock.hpp"
#include "render/SceneMock.hpp"
#include "gfx/render/SceneManager.hpp"
#include "logger/Logger.hpp"

using namespace ::testing;
namespace
{
he::gfx::render::Scene scene1("Main Menu");
he::gfx::render::Scene scene2("Game");
} // namespace

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FIXTURE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace  he::gfx::render
{
class TestedSceneManager : public SceneManager
{
public:
    TestedSceneManager(const SceneTransitionTable& transitionTable) : SceneManager(transitionTable)
    {
    }

    IScene* getCurrentScene() const
    {
        return m_currentScene;
    }

    const std::size_t getNumberOfScenes() const
    {
        return m_scenes.size();
    }
};

class SceneManagerTests : public testing::Test
{
public:
    SceneManagerTests() = default;
    ~SceneManagerTests() override {};

    void createSut(const SceneTransitionTable& transitionTable)
    {
        sut = std::make_unique<he::gfx::render::TestedSceneManager>(transitionTable);
    }

    std::unique_ptr<he::gfx::render::TestedSceneManager> sut;

    std::shared_ptr<he::gfx::render::SceneMock> sceneMock1 = std::make_shared<he::gfx::render::SceneMock>();
    std::shared_ptr<he::gfx::render::SceneMock> sceneMock2 = std::make_shared<he::gfx::render::SceneMock>();
    std::shared_ptr<he::gfx::render::SceneMock> sceneMock3 = std::make_shared<he::gfx::render::SceneMock>();
    std::shared_ptr<he::gfx::render::Scene> scenePtr1 = std::make_shared<he::gfx::render::Scene>(scene1);
    std::shared_ptr<he::gfx::render::Scene> scenePtr2 = std::make_shared<he::gfx::render::Scene>(scene2);
    std::vector<std::shared_ptr<IScene>> scenes{scenePtr1, scenePtr2};
    he::gfx::render::RenderMock renderMock;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TEST
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_F(SceneManagerTests, whenGetNotInitializedsecondScene_shouldNotThrowException)
{
    he::gfx::render::SceneTransitionTable transitionTable{{scenePtr1, nullptr, nullptr, nullptr, nullptr}};

    EXPECT_NO_THROW((he::gfx::render::SceneManager(transitionTable)));
}

TEST_F(SceneManagerTests, whenGetEmptySetOfTransitions_shouldThrowException)
{
    he::gfx::render::SceneTransitionTable transitionTable{};

    EXPECT_THROW((he::gfx::render::SceneManager(transitionTable)), he::common::invalid_initialization);
}

TEST_F(SceneManagerTests, whenRender_thenChangeOpenCloseState_shouldRenderFurther)
{
    EXPECT_CALL(*sceneMock1, open());
    EXPECT_CALL(*sceneMock1, getName()).WillRepeatedly(Return("Main Menu"));
    EXPECT_CALL(*sceneMock1, isOpen()).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*sceneMock1, render(_)).Times(1);
    EXPECT_CALL(*sceneMock1, close());

    EXPECT_CALL(*sceneMock2, open());
    EXPECT_CALL(*sceneMock2, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*sceneMock2, getName()).WillRepeatedly(Return("Game"));
    EXPECT_CALL(*sceneMock2, render(_)).Times(1);

    he::gfx::render::SceneTransitionTable transitionTable{{sceneMock1, nullptr, sceneMock2, nullptr, nullptr}};
    createSut(transitionTable);

    sut->render(renderMock);
    sceneMock1->close();
    sceneMock2->open();
    sut->render(renderMock);
    ASSERT_EQ(sut->getCurrentScene()->getName(), scenePtr2->getName());
}

TEST_F(SceneManagerTests, whenRender_thenChangeOpenCloseStateToClose_shouldThrowException)
{
    EXPECT_CALL(*sceneMock1, open());
    EXPECT_CALL(*sceneMock1, getName()).WillRepeatedly(Return("Main Menu"));
    EXPECT_CALL(*sceneMock1, isOpen()).WillOnce(Return(true)).WillOnce(Return(false)).WillOnce(Return(false));
    EXPECT_CALL(*sceneMock1, render(_)).Times(1);
    EXPECT_CALL(*sceneMock1, close());

    EXPECT_CALL(*sceneMock2, getName()).WillRepeatedly(Return("Game"));
    EXPECT_CALL(*sceneMock2, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*sceneMock2, render(_)).Times(0);

    he::gfx::render::SceneTransitionTable transitionTable{{sceneMock1, nullptr, sceneMock2, nullptr, nullptr}};
    createSut(transitionTable);

    sut->render(renderMock);
    sceneMock1->close();
    EXPECT_THROW(sut->render(renderMock), he::common::invalid_initialization);
}

TEST_F(SceneManagerTests, whenEventInput_thenChangeScenesAndRender)
{
    he::window::Event event{};
    event.type = he::window::Event::keyPressed;
    event.key.code = he::window::Keyboard::ENTER;

    he::gfx::render::SceneTransitionTable transitionTable{{scenePtr1, he::event_to_ptr<he::window::Event>(event), scenePtr2, nullptr, nullptr}};
    createSut(transitionTable);

    sut->render(renderMock);
    sut->process_event(event);
    EXPECT_NO_THROW(sut->render(renderMock));
    ASSERT_EQ(sut->getCurrentScene()->getName(), scenePtr2->getName());
}
} // namespace he::gfx::render