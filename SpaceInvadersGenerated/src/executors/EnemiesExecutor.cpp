#include "EnemiesExecutor.h"

namespace Game::ExecutionBlock
{

    void Enemies::loadContext() noexcept
    {
        this->parametersContext.load();

        this->renderCmdsContext.load();
        this->unitsContext.load();
        this->bulletsToSpawnContext.load();
    }

    void Enemies::initiate() noexcept
    {
        this->initImpl();
    }

    void Enemies::update(float dt) noexcept
    {
        this->updateImpl(dt);
    }

    void Enemies::flushSharedLocalContexts() noexcept
    {
        this->renderCmdsContext.reset();
        this->bulletsToSpawnContext.reset();
    }

}
