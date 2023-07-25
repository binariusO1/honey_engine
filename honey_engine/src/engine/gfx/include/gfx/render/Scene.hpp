#pragma once

#include "exception/invalid_initialization.hpp"
#include "gfx/render/BaseLayer.hpp"
#include "gfx/render/IScene.hpp"
#include "gfx/render/OpenCloseState.hpp"

namespace he
{
namespace gfx
{
namespace render
{

class Scene : public IScene
{
private:
    struct Context
    {
        Context(const std::string& name) : name(name)
        {
            if (name.empty())
            {
                throw he::common::invalid_initialization("Name cannot be empty!");
            }
        }
        const std::string name;
        OpenCloseState state{OpenCloseState::closed};
    };

public:
    Scene(const std::string&);
    ~Scene() override;

public:
    void open() override;
    void close() override;
    bool isOpen() const override;

public:
    void addLayer(const std::shared_ptr<he::gfx::render::ILayer>&);
    void addLayers(const LayersList&);
    void removeLayer(const std::shared_ptr<he::gfx::render::ILayer>&);

public:
    void render(gfx::render::IRender&) override;

public:
    const std::string getName() override;

protected:
    Context m_context;
    LayersMap m_layers;
};
} // namespace render
} // namespace gfx
} // namespace he