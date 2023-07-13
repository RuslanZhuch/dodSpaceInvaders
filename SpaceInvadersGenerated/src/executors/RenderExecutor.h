#pragma once

#include <Contexts/ApplicationContext.h>
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
        Context::Application::Data applicationContext;
    };
}
