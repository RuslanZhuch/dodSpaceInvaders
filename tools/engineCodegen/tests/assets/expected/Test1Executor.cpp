#include "Test1Executor.h"

namespace Game::ExecutionBlock
{

    void Test1::loadContext() noexcept
    {

    }

    void Test1::initiate() noexcept
    {
        this->target1Context.init();
        this->target2Context.init();
        this->target3Context.init();
    }

    void Test1::update(float dt) noexcept
    {

    }

    void Test1::flushSharedLocalContexts() noexcept
    {
        this->target1Context.reset();
        this->target3Context.reset();
    }

}
