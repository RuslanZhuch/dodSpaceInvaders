#include "EnemyBulletsExecutor.h"

#include <dod/BufferUtils.h>

#include <internal/gameplay/BulletsCore.h>
#include <internal/gameplay/BulletsGameplay.h>
#include <internal/gameplay/CollisionCore.h>

#include <iostream>

namespace Game::ExecutionBlock
{

    void EnemyBullets::initImpl() noexcept
    {
        Dod::BufferUtils::constructBack(this->renderCmdsContext.modelsMeta);
    }

    void EnemyBullets::updateImpl(float dt) noexcept
    {

        const auto bulletsToCreateX{ Dod::SharedContext::get(this->toSpawnContext).xCoords };
        const auto bulletsToCreateY{ Dod::SharedContext::get(this->toSpawnContext).yCoords };

        Dod::BufferUtils::populate(this->soundCmdsContext.soundIdsToPlay, 1,
            Dod::BufferUtils::getNumFilledElements(bulletsToCreateX));

        Dod::BufferUtils::append(this->renderCmdsContext.xCoords,
            Dod::BufferUtils::createImFromBuffer(bulletsToCreateX));
        Dod::BufferUtils::append(this->renderCmdsContext.yCoords,
            Dod::BufferUtils::createImFromBuffer(bulletsToCreateY));

        Game::Core::Bullets::updateMovement(
            this->renderCmdsContext.yCoords,
            this->parametersContext.velocity,
            dt
        );

        const auto obstaclesXCorrds{ Dod::SharedContext::get(this->obstaclesCoordsContext).xCoords };
        const auto obstaclesYCorrds{ Dod::SharedContext::get(this->obstaclesCoordsContext).yCoords };

        const auto obstaclesWidth{ this->obstacleDimentionsContext.width };
        const auto obstaclesHeight{ this->obstacleDimentionsContext.height };

        Game::Gameplay::Bullets::testWithObstacles(
            this->toRemoveContext.ids,
            this->obstaclesToHitContext.ids,
            Dod::BufferUtils::createImFromBuffer(this->renderCmdsContext.xCoords),
            Dod::BufferUtils::createImFromBuffer(this->renderCmdsContext.yCoords),
            Dod::BufferUtils::createImFromBuffer(obstaclesXCorrds),
            Dod::BufferUtils::createImFromBuffer(obstaclesYCorrds),
            obstaclesWidth,
            obstaclesHeight
        );
        for (int32_t id{}; id < Dod::BufferUtils::getNumFilledElements(this->toRemoveContext.ids); ++id)
        {
            std::cout << "Enemy bullet to remove id " << Dod::BufferUtils::get(this->toRemoveContext.ids, id) << "\n";
        }
        const auto playerX{ Dod::SharedContext::get(this->playerCoordContext).xCoords };
        const auto playerY{ Dod::SharedContext::get(this->playerCoordContext).yCoords };
        const auto playerWidth{ this->playerDimentionsContext.width };
        const auto playerHeight{ this->playerDimentionsContext.height };

        Game::Gameplay::Bullets::collisionUpdate(
            this->playerToHitContext.ids,
            this->toRemoveContext.ids,
            Dod::BufferUtils::createImFromBuffer(this->renderCmdsContext.xCoords),
            Dod::BufferUtils::createImFromBuffer(this->renderCmdsContext.yCoords),
            Dod::BufferUtils::createImFromBuffer(playerX),
            Dod::BufferUtils::createImFromBuffer(playerY),
            playerWidth,
            playerHeight
        );

        Game::Core::Collision::pointsPlaneIntersection(
            this->toRemoveContext.ids,
            Dod::BufferUtils::createImFromBuffer(this->renderCmdsContext.yCoords),
            this->sceneParametersContext.bottomPlaneY,
            this->sceneParametersContext.bottomPlaneDir
        );

        Game::Core::Bullets::updateLifetime(
            this->toRemoveContext.ids,
            this->renderCmdsContext.xCoords,
            this->renderCmdsContext.yCoords
        );

        Dod::BufferUtils::get(this->renderCmdsContext.modelsMeta, 0).modelId = 2;
        Dod::BufferUtils::get(this->renderCmdsContext.modelsMeta, 0).numOfElements = Dod::BufferUtils::getNumFilledElements(this->renderCmdsContext.xCoords);

    }

}
