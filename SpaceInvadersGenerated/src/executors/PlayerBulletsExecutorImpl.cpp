#include "PlayerBulletsExecutor.h"

#include <dod/BufferUtils.h>

#include <internal/gameplay/BulletsCore.h>
#include <internal/gameplay/BulletsGameplay.h>
#include <internal/gameplay/CollisionCore.h>

#include <iostream>

namespace Game::ExecutionBlock
{

    void PlayerBullets::initImpl() noexcept
    {
        Dod::BufferUtils::constructBack(this->renderCmdsContext.modelsMeta);
    }

    void PlayerBullets::updateImpl(float dt) noexcept
    {

        const auto bulletsToCreateX{ Dod::SharedContext::get(this->toSpawnContext).xCoords };
        const auto bulletsToCreateY{ Dod::SharedContext::get(this->toSpawnContext).yCoords };

        Dod::BufferUtils::populate(this->soundCmdsContext.soundIdsToPlay, 0,
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
        for (int32_t id{}; id < Dod::BufferUtils::getNumFilledElements(this->obstaclesToHitContext.ids); ++id)
        {
            std::cout << "Obstacle to remove id " << Dod::BufferUtils::get(this->obstaclesToHitContext.ids, id) << "\n";
        }

        Game::Core::Collision::pointsPlaneIntersection(
            this->toRemoveContext.ids,
            Dod::BufferUtils::createImFromBuffer(this->renderCmdsContext.yCoords),
            this->sceneParametersContext.topPlaneY,
            this->sceneParametersContext.topPlaneDir
        );

        const auto enemiesX{ Dod::SharedContext::get(this->enemiesCoordsContext).xCoords };
        const auto enemiesY{ Dod::SharedContext::get(this->enemiesCoordsContext).yCoords };

        Game::Gameplay::Bullets::collisionUpdate(
            this->enemiesToHitContext.ids,
            this->toRemoveContext.ids,
            Dod::BufferUtils::createImFromBuffer(this->renderCmdsContext.xCoords),
            Dod::BufferUtils::createImFromBuffer(this->renderCmdsContext.yCoords),
            Dod::BufferUtils::createImFromBuffer(enemiesX),
            Dod::BufferUtils::createImFromBuffer(enemiesY),
            this->enemyDimentionsContext.width,
            this->enemyDimentionsContext.height
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
