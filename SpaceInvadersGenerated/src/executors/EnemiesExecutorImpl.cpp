#include "EnemiesExecutor.h"

#include <dod/BufferUtils.h>

#include <internal/gameplay/EnemiesGameplay.h>

#include <iostream>
#include <format>

namespace Game::ExecutionBlock
{

    void Enemies::initImpl() noexcept
    {
        Dod::BufferUtils::constructBack(this->renderCmdsContext.modelsMeta);

        Game::Gameplay::Enemies::generateEnemies(
            this->parametersContext.numOfEnemiesPerRow,
            this->parametersContext.numOfEnemiesPerCol,
            this->parametersContext.enemiesXStride,
            this->parametersContext.enemiesYStride,
            this->parametersContext.startCoordX,
            this->parametersContext.startCoordY,
            this->unitsContext.xCoords,
            this->unitsContext.yCoords
        );
    }

    void Enemies::updateImpl(float dt) noexcept
    {

        const auto toRemove{ Dod::SharedContext::get(this->toHitContext).ids };
        Game::Gameplay::Enemies::enemiesLifetimeUpdate(
            Dod::BufferUtils::createImFromBuffer(toRemove),
            this->unitsContext.xCoords,
            this->unitsContext.yCoords
        );

        Game::Gameplay::Enemies::enemiesUpdate(
            dt,
            this->parametersContext.cooldownTimeLeft,
            this->parametersContext.direction,
            this->parametersContext.targetX,
            this->parametersContext.startCoordX,
            this->parametersContext.startCoordY,
            this->unitsContext.xCoords,
            this->unitsContext.yCoords
        );

        Dod::BufferUtils::append(this->renderCmdsContext.xCoords, Dod::BufferUtils::createImFromBuffer(this->unitsContext.xCoords));
        Dod::BufferUtils::append(this->renderCmdsContext.yCoords, Dod::BufferUtils::createImFromBuffer(this->unitsContext.yCoords));

        Dod::BufferUtils::constructBack(this->renderCmdsContext.modelsMeta);
        Dod::BufferUtils::get(this->renderCmdsContext.modelsMeta, 0).modelId = 1;
        Dod::BufferUtils::get(this->renderCmdsContext.modelsMeta, 0).numOfElements = Dod::BufferUtils::getNumFilledElements(this->renderCmdsContext.xCoords);

        const auto numOfEnemyBulletsToCreate{ Game::Gameplay::Enemies::updateEnemyBulletsCreation(
            dt,
            this->parametersContext.weaponCooldownTimeLeft,
            Dod::BufferUtils::getNumFilledElements(this->unitsContext.xCoords)
        ) };

        Game::Gameplay::Enemies::generateEnemyBullets(
            numOfEnemyBulletsToCreate,
            this->bulletsToSpawnContext.xCoords,
            this->bulletsToSpawnContext.yCoords,
            this->parametersContext.rand,
            Dod::BufferUtils::createImFromBuffer(this->unitsContext.xCoords),
            Dod::BufferUtils::createImFromBuffer(this->unitsContext.yCoords)
        );

    }

}
