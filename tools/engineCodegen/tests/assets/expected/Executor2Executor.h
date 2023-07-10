#pragma once

#include <Contexts/Scontext1Context.h>
#include <Contexts/Scontext2Context.h>
#include <Contexts/Type1Context.h>
#include <Contexts/Type2Context.h>
#include <Contexts/Type3Context.h>

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{
    class Executor2
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
        Context::Type1::Data local1Context;
        Context::Type2::Data local2Context;
        Context::Type2::Data local3Context;
        Context::Type1::Data target1Context;
        Context::Type3::Data target2Context;
        Context::Type3::Data target3Context;
        const Dod::SharedContext::Controller<Context::Scontext1::Data>* shared1Context{ nullptr };
        const Dod::SharedContext::Controller<Context::Scontext1::Data>* shared2Context{ nullptr };
        const Dod::SharedContext::Controller<Context::Scontext2::Data>* shared3Context{ nullptr };
        const Dod::SharedContext::Controller<Context::Scontext2::Data>* shared4Context{ nullptr };
    };
}
