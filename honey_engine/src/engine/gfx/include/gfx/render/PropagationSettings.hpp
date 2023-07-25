#pragma once

#include <cstdint>
#include <utility>

namespace he
{
namespace gfx
{
struct PropagationSettings
{
    std::uint32_t numberOfElementsX{1};
    std::uint32_t numberOfElementsY{1};
    float distanceX{0};
    float distanceY{0};
}; 
} // namespace gfx
} // namespace he