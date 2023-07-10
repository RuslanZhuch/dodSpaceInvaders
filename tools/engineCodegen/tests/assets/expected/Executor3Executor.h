#pragma once

#include <Contexts/Scontext1Context.h>
#include <Contexts/Scontext2Context.h>

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{
    class Executor3
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
        const Dod::SharedContext::Controller<Context::Scontext1::Data>* shared1Context{ nullptr };
        const Dod::SharedContext::Controller<Context::Scontext2::Data>* shared3Context{ nullptr };
    };
}
