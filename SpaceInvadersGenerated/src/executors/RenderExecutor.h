#pragma once

#include <Contexts/ApplicationContext.h>
#include <Contexts/CommonContext.h>
#include <Contexts/ModelsToRenderContext.h>
#include <Contexts/RenderCommandsContext.h>
#include <Contexts/RenderInternalContext.h>

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{
    class Render
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
        Context::RenderInternal::Data rendererContext;
        Context::Common::Data windowSpecsContext;
        Context::RenderInternal::Data internalContext;
        Context::Application::Data applicationContext;
        const Dod::SharedContext::Controller<Context::ModelsToRender::Data>* modelsIdsContext{ nullptr };
        const Dod::SharedContext::Controller<Context::RenderCommands::Data>* renderCmdsContext{ nullptr };
    };
}
