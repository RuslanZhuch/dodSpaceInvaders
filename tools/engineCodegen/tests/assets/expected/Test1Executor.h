#pragma once

#include <dod/MemPool.h>

namespace Game::ExecutionBlock
{

    class Test1
    {

    public:
        void loadContext();

        void initiate();
        void update(float dt);

        void flushSharedLocalContexts();

    private:
        Dod::MemPool memory;
    };

};
