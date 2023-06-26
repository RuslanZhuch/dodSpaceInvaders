#include "EnemiesExecution.h"
#include "EnemiesGameplay.h"

#include <fstream>
#include <iostream>

#include <dod/MemUtils.h>
#include <dod/BufferUtils.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>

template <>
const Game::Context::Sounds::Shared& Game::ExecutionBlock::Enemies::getSharedLocalContext<Game::Context::Sounds::Shared>()
{
    return this->soundsContext;
}
template <>
const Game::Context::Render::Shared& Game::ExecutionBlock::Enemies::getSharedLocalContext<Game::Context::Render::Shared>()
{
    return this->renderContext;
}
template <>
const Game::Context::BulletsToSpawn::Shared& Game::ExecutionBlock::Enemies::getSharedLocalContext<Game::Context::BulletsToSpawn::Shared>()
{
    return this->bulletsToSpawnContext;
}
template <>
const Game::Context::Units::Shared& Game::ExecutionBlock::Enemies::getSharedLocalContext<Game::Context::Units::Shared>()
{
    return this->unitsContext;
}

void Game::ExecutionBlock::Enemies::loadContext()
{

    this->enemyContext.load();

}

void Game::ExecutionBlock::Enemies::initiate()
{

    this->renderContext.load();
    Dod::BufferUtils::constructBack(this->renderContext.modelsMeta);

    this->bulletsToSpawnContext.load();

    this->unitsContext.load();
    this->unitsContext.width = 15.f;
    this->unitsContext.height = 20.f;

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
