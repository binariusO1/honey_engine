#pragma once

#include <string>
#include "gfx/render/OpenCloseState.hpp"







namespace he
{
namespace gfx
{
class Event
{
public:
    struct SceneEvent
    {
        const char*     name;
        OpenCloseState      state;
    };

    enum EventType
    {
        none,
        sceneTransition
    };

    EventType type;     ///< Type of the event

    union
    {
        SceneEvent      sceneEvent;
    };
};
} // namespace gfx
} // namespace he