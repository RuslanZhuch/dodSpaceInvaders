#include "PlayerExecutor.h"

namespace Game::ExecutionBlock
{

    void Player::loadContext() noexcept
    {
        this->parametersContext.load();

        this->unitContext.load();
        this->renderCmdsContext.load();
        this->bulletsToSpawnContext.load();
    }

    void Player::initiate() noexcept
    {
        this->initImpl();
    }

    void Player::update(float dt) noexcept
    {
        this->updateImpl(dt);
    }

    void Player::flushSharedLocalContexts() noexcept
    {
        this->renderCmdsContext.reset();
        this->bulletsToSpawnContext.reset();
    }

}
