#include "EnemyBulletsExecutor.h"

namespace Game::ExecutionBlock
{

    void EnemyBullets::loadContext() noexcept
    {
        this->sceneParametersContext.load();
        this->parametersContext.load();
        this->toRemoveContext.load();
        this->obstacleDimentionsContext.load();
        this->playerDimentionsContext.load();

        this->renderCmdsContext.load();
        this->obstaclesToHitContext.load();
        this->playerToHitContext.load();
        this->soundCmdsContext.load();
    }

    void EnemyBullets::initiate() noexcept
    {
        this->initImpl();
    }

    void EnemyBullets::update(float dt) noexcept
    {
        this->updateImpl(dt);
    }

    void EnemyBullets::flushSharedLocalContexts() noexcept
    {
        this->toRemoveContext.reset();
        this->obstaclesToHitContext.reset();
        this->playerToHitContext.reset();
        this->soundCmdsContext.reset();
    }

}
