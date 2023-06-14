#pragma once

#include <dod/MemPool.h>

namespace Game::ExecutionBlock
{
    class Test1
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
        Game::Context::Type1::Local local1Context;
        Game::Context::Type2::Local local2Context;
        Game::Context::Type3::Local local3Context;
        Game::Context::Type3::Local local4Context;
        Game::Context::Type4::Shared target1Context;
        Game::Context::Type4::Shared target2Context;
        Game::Context::Type4::Shared target3Context;
        const Dod::SharedContext::Controller<Game::Context::Type5::Shared>* shared1Context{ nullptr };
        const Dod::SharedContext::Controller<Game::Context::Type5::Shared>* shared2Context{ nullptr };
        const Dod::SharedContext::Controller<Game::Context::Type6::Shared>* shared3Context{ nullptr };
    };
}
