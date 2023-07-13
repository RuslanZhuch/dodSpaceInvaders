#include "RenderExecutor.h"

namespace Game::ExecutionBlock
{

    void Render::loadContext() noexcept
    {
        this->rendererContext.load();

        this->applicationContext.load();
    }

    void Render::initiate() noexcept
    {
        this->initImpl();
    }

    void Render::update(float dt) noexcept
    {
        this->updateImpl(dt);
    }

    void Render::flushSharedLocalContexts() noexcept
    {
        this->applicationContext.reset();
    }

}
