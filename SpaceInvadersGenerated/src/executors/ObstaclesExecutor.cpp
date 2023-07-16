#include "ObstaclesExecutor.h"

namespace Game::ExecutionBlock
{

    void Obstacles::loadContext() noexcept
    {
        this->obstaclesCoordsContext.load();
        this->parametersContext.load();
        this->dimentionsContext.load();
        this->toRemoveContext.load();

        this->renderCmdsContext.load();
    }

    void Obstacles::initiate() noexcept
    {
        this->initImpl();
    }

    void Obstacles::update(float dt) noexcept
    {
        this->updateImpl(dt);
    }

    void Obstacles::flushSharedLocalContexts() noexcept
    {
        this->renderCmdsContext.reset();
        this->toRemoveContext.reset();
    }

}
