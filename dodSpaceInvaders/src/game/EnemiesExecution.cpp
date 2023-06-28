#include "EnemiesExecution.h"
#include "EnemiesGameplay.h"

#include <fstream>
#include <iostream>

#include <dod/MemUtils.h>
#include <dod/BufferUtils.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>

void Game::ExecutionBlock::Enemies::loadContext()
{

    this->enemyContext.load();
    this->renderContext.load();
    this->unitsContext.load();
    this->bulletsToSpawnContext.load();

}

void Game::ExecutionBlock::Enemies::initiate()
{

    Dod::BufferUtils::constructBack(this->renderContext.modelsMeta);


//    this->unitsContext.width = 15.f;
//    this->unitsContext.height = 20.f;

    Game::Gameplay::Enemies::generateEnemies(
        this->enemyContext.numOfEnemiesPerRow,
        this->enemyContext.numOfEnemiesCols,
        this->enemyContext.enemiesXStride,
        this->enemyContext.enemiesYStride,
        this->enemyContext.batchCoordX,
        this->enemyContext.batchCoordY,
        this->unitsContext.xCoords,
        this->unitsContext.yCoords
    );

}

void Game::ExecutionBlock::Enemies::update(float dt)
{

    const auto toRemove{ Dod::SharedContext::get(this->toHitSContext).objectsToHit };
    Game::Gameplay::Enemies::enemiesLifetimeUpdate(
        Dod::BufferUtils::createImFromBuffer(toRemove),
        this->unitsContext.xCoords,
        this->unitsContext.yCoords
    );

    Game::Gameplay::Enemies::enemiesUpdate(
        dt,
        this->enemyContext.batchMoveTimeleft,
        this->enemyContext.direction,
        this->enemyContext.batchTargetX,
        this->enemyContext.batchCoordX,
        this->enemyContext.batchCoordY,
        this->unitsContext.xCoords,
        this->unitsContext.yCoords
    );

    Dod::BufferUtils::append(this->renderContext.xCoords, Dod::BufferUtils::createImFromBuffer(this->unitsContext.xCoords));
    Dod::BufferUtils::append(this->renderContext.yCoords, Dod::BufferUtils::createImFromBuffer(this->unitsContext.yCoords));

    Dod::BufferUtils::constructBack(this->renderContext.modelsMeta);
    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).modelId = 1;
    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).numOfElements = Dod::BufferUtils::getNumFilledElements(this->renderContext.xCoords);

    const auto numOfEnemyBulletsToCreate{ Game::Gameplay::Enemies::updateEnemyBulletsCreation(
        dt,
        this->enemyContext.enemyWeaponCooldownTimeLeft,
        Dod::BufferUtils::getNumFilledElements(this->unitsContext.xCoords)
    ) };

    Game::Gameplay::Enemies::generateEnemyBullets(
        numOfEnemyBulletsToCreate,
        this->bulletsToSpawnContext.xCoords,
        this->bulletsToSpawnContext.yCoords,
        this->enemyContext.rand,
        Dod::BufferUtils::createImFromBuffer(this->unitsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->unitsContext.yCoords)
    );

}

void Game::ExecutionBlock::Enemies::flushSharedLocalContexts()
{
    this->soundsContext.reset();
    this->bulletsToSpawnContext.reset();
    this->renderContext.reset();
}
