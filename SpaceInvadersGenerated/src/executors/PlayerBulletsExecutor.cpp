#include "PlayerBulletsExecutor.h"

namespace Game::ExecutionBlock
{

    void PlayerBullets::loadContext() noexcept
    {
        this->sceneParametersContext.load();
        this->parametersContext.load();
        this->toRemoveContext.load();
        this->obstacleDimentionsContext.load();
        this->enemyDimentionsContext.load();

        this->renderCmdsContext.load();
        this->obstaclesToHitContext.load();
        this->enemiesToHitContext.load();
        this->soundCmdsContext.load();
    }

    void PlayerBullets::initiate() noexcept
    {
        this->initImpl();
    }

    void PlayerBullets::update(float dt) noexcept
    {
        this->updateImpl(dt);
    }

    void PlayerBullets::flushSharedLocalContexts() noexcept
    {
        this->toRemoveContext.reset();
        this->obstaclesToHitContext.reset();
        this->enemiesToHitContext.reset();
        this->soundCmdsContext.reset();
    }

}
