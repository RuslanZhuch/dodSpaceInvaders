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

    private:
        Dod::MemPool memory;
    };
}
