#pragma once

#include <Contexts/CommonContext.h>
#include <Contexts/ModelsToRenderContext.h>
#include <Contexts/PlayerDimentionsContext.h>
#include <Contexts/SceneContext.h>

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{
    class Models
    {
    public:
        void loadContext() noexcept;
        void initiate() noexcept;
        void update(float dt) noexcept;
        void flushSharedLocalContexts() noexcept;

    private:
        void initImpl() noexcept;
        void updateImpl(float dt) noexcept;

    public:
        Dod::MemPool memory;
        Context::PlayerDimentions::Data playerParamsContext;
        Context::Scene::Data sceneParamsContext;
        Context::Common::Data windowParamsContext;
        Context::ModelsToRender::Data modelsContext;
    };
}
