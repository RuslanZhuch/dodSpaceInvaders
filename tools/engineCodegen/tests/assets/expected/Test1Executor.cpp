#include "Test1Executor.h"

namespace Game::ExecutionBlock
{

    void Test1::loadContext() noexcept
    {
        int32_t header{};
        this->memory.allocate(2048);

        this->target1Context.load(this->memory, header);
        this->target2Context.load(this->memory, header);
        this->target3Context.load(this->memory, header);
    }

    void Test1::initiate() noexcept
    {
        this->target1Context.init();
        this->target2Context.init();
        this->target3Context.init();

        this->initImpl();
    }

    void Test1::update(float dt) noexcept
    {
        this->updateImpl(dt);
    }

    void Test1::flushSharedLocalContexts() noexcept
    {
        this->target1Context.reset();
        this->target3Context.reset();
    }

}
