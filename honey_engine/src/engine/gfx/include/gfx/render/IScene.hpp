#pragma once

#include <string>
#include "gfx/render/IRenderable.hpp"

namespace he
{
namespace gfx
{
namespace render
{

class IScene : public IRenderable
{
public:
    IScene() = default;
    ~IScene() = default;

public:
    virtual void open() = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;

public:
    virtual void render(gfx::render::IRender&) = 0;

public:
    virtual const std::string getName() = 0;
};
} // namespace render
} // namespace gfx
} // namespace he