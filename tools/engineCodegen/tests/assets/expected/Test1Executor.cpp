#include "Test1Executor.h"

namespace Game::ExecutionBlock
{

    void Test1::loadContext() noexcept
    {
        int32_t header{};
        this->memory.allocate(2048);

        this->local1Context.load(this->memory, header);
        this->local2Context.load(this->memory, header);
        this->local3Context.load(this->memory, header);
        this->local4Context.load(this->memory, header);

        this->target1Context.load();
        this->target2Context.load();
        this->target3Context.load();
    }

    void Test1::initiate() noexcept
    {
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
