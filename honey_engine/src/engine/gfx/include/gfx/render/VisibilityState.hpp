#pragma once

#include <cstdint>

namespace he
{
namespace gfx
{
enum class VisibilityState : std::uint8_t
{
    hidden = 0,
    visible
};
} // namespace gfx
} // namespace he