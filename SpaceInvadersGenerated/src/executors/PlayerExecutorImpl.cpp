#include "PlayerExecutor.h"

#include <dod/BufferUtils.h>

#include <internal/gameplay/PlayerGameplay.h>
#include <internal/gameplay/Inputs.h>

namespace Game::ExecutionBlock
{

    void Player::initImpl() noexcept
    {
        Dod::BufferUtils::constructBack(this->unitContext.xCoords, this->parametersContext.positionX);
        Dod::BufferUtils::constructBack(this->unitContext.yCoords, this->parametersContext.positionY);
    }

    void Player::updateImpl(float dt) noexcept
    {

        const auto toRemove{ Dod::SharedContext::get(this->toHitContext).ids};

        Game::Gameplay::Player::lifetimeUpdate(
            Dod::BufferUtils::createImFromBuffer(toRemove),
            this->parametersContext.lifes
        );

        Game::Gameplay::Player::updateInputs(
            this->parametersContext.inputs,
            this->parametersContext.prevInputs
        );

        auto& currX{ Dod::BufferUtils::get(this->unitContext.xCoords, 0) };
        auto& currY{ Dod::BufferUtils::get(this->unitContext.yCoords, 0) };

        Game::Gameplay::Player::updateMovement(
            this->parametersContext.move,
            currX,
            this->parametersContext.inputs,
            this->parametersContext.prevInputs,
            dt
        );

        Dod::BufferUtils::constructBack(this->renderCmdsContext.modelsMeta, Models::ModelMeta(4, 1), this->parametersContext.lifes > 0);

        Dod::BufferUtils::constructBack(this->renderCmdsContext.xCoords, currX, this->parametersContext.lifes > 0);
        Dod::BufferUtils::constructBack(this->renderCmdsContext.yCoords, currY, this->parametersContext.lifes > 0);

        const auto numOfPlayerBulletsToCreate{ Game::Gameplay::Player::updateFireComponent(
            this->parametersContext.lifes,
            this->parametersContext.inputs,
            this->parametersContext.prevInputs
        ) };

        Game::Gameplay::Player::createBullets(
            numOfPlayerBulletsToCreate,
            this->bulletsToSpawnContext.xCoords,
            this->bulletsToSpawnContext.yCoords,
            currX,
            currY
        );

    }

}
