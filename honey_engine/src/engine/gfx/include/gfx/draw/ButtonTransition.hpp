#pragma once

#include <string>
#include "gfx/draw/ButtonState.hpp"

namespace he
{
namespace gfx
{
struct ButtonTransition
{
    ButtonTransition() : m_currentState{ButtonState::Idle} {}
    ButtonTransition(const ButtonState oldState, const ButtonState newState) : m_oldState{oldState} , m_currentState{newState} {}
    ButtonTransition(const ButtonTransition&) = default;
    void update(const ButtonState state)
    {
        m_oldState = m_currentState;
        m_currentState = state;
    }
    ButtonState getCurrentState() const { return m_currentState; }
    ButtonState getOldState() const { return m_oldState; }

private:
    ButtonState m_oldState;
    ButtonState m_currentState;
};

inline bool operator ==(const ButtonTransition& left, const ButtonTransition& right)
{
    return (left.getOldState() == right.getOldState()) && (left.getCurrentState() == right.getCurrentState());
}

struct ButtonTransitionHasher
{
    std::size_t operator()(const ButtonTransition& transition) const
    {
        std::size_t h1 = std::hash<std::string>{}(toString(transition.getOldState()));
        std::size_t h2 = std::hash<std::string>{}(toString(transition.getCurrentState()));
        return h1 ^ (h2 << 1);
    }
};
} // namespace gfx
} // namespace he