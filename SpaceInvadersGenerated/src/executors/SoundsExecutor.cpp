#include "SoundsExecutor.h"

namespace Game::ExecutionBlock
{

    void Sounds::loadContext() noexcept
    {
        this->coreContext.load();

    }

    void Sounds::initiate() noexcept
    {
        this->initImpl();
    }

    void Sounds::update(float dt) noexcept
    {
        this->updateImpl(dt);
    }

    void Sounds::flushSharedLocalContexts() noexcept
    {
    }

}
