#pragma once

#include <Contexts/SoundCmdsContext.h>
#include <Contexts/SoundsContext.h>

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{
    class Sounds
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
        Context::Sounds::Data coreContext;
        const Dod::SharedContext::Controller<Context::SoundCmds::Data>* cmdsContext{ nullptr };
    };
}
