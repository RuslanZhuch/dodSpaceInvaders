#include "PlayerBulletsExecutor.h"
#include "BulletsGameplay.h"
#include "BulletsCore.h"
#include "CollisionCore.h"

#include <fstream>
#include <iostream>

#include <dod/MemUtils.h>
#include <dod/BufferUtils.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>

void Game::ExecutionBlock::PlayerBullets::loadContext()
{

    this->renderContext.load();
    this->soundsContext.load();
    this->obstaclesToHitContext.load();
    this->obstacleParameters.load();
    this->enemiesToHitContext.load();
    this->playerBulletsToRemove.load();
    this->enemyDimentions.load();

    this->sceneParameters.load();
    this->playerBulletsContext.load();

}

void Game::ExecutionBlock::PlayerBullets::initiate()
{

    Dod::BufferUtils::constructBack(this->renderContext.modelsMeta);

}

void Game::ExecutionBlock::PlayerBullets::update(float dt)
{

    const auto bulletsToCreateX{ Dod::SharedContext::get(this->bulletsSContext).xCoords };
    const auto bulletsToCreateY{ Dod::SharedContext::get(this->bulletsSContext).yCoords };

    Dod::BufferUtils::populate(this->soundsContext.soundIdsToPlay, 0,
        Dod::BufferUtils::getNumFilledElements(bulletsToCreateX));

    Dod::BufferUtils::append(this->renderContext.xCoords,
        Dod::BufferUtils::createImFromBuffer(bulletsToCreateX));
    Dod::BufferUtils::append(this->renderContext.yCoords,
        Dod::BufferUtils::createImFromBuffer(bulletsToCreateY));

    Game::Core::Bullets::updateMovement(
        this->renderContext.yCoords,
        this->playerBulletsContext.velocity,
        dt
    );

    const auto obstaclesXCorrds{ Dod::SharedContext::get(this->obstaclesCoordinates).xCoords };
    const auto obstaclesYCorrds{ Dod::SharedContext::get(this->obstaclesCoordinates).yCoords };

    const auto obstaclesWidth{ this->obstacleParameters.width };
    const auto obstaclesHeight{ this->obstacleParameters.height };

    Game::Gameplay::Bullets::testWithObstacles(
        this->playerBulletsToRemove.objectsToHit,
        this->obstaclesToHitContext.objectsToHit,
        Dod::BufferUtils::createImFromBuffer(this->renderContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->renderContext.yCoords),
        Dod::BufferUtils::createImFromBuffer(obstaclesXCorrds),
        Dod::BufferUtils::createImFromBuffer(obstaclesYCorrds),
        obstaclesWidth,
        obstaclesHeight
    );
    for (int32_t id{}; id < Dod::BufferUtils::getNumFilledElements(this->obstaclesToHitContext.objectsToHit); ++id)
    {
        std::cout << "Obstacle to remove id " << Dod::BufferUtils::get(this->obstaclesToHitContext.objectsToHit, id) << "\n";
    }

    Game::Core::Collision::pointsPlaneIntersection(
        this->playerBulletsToRemove.objectsToHit,
        Dod::BufferUtils::createImFromBuffer(this->renderContext.yCoords),
        this->sceneParameters.topPlaneY,
        this->sceneParameters.topPlaneDir
    );

    const auto enemiesX{ Dod::SharedContext::get(this->enemiesSContext).xCoords };
    const auto enemiesY{ Dod::SharedContext::get(this->enemiesSContext).yCoords };

    Game::Gameplay::Bullets::collisionUpdate(
        this->enemiesToHitContext.objectsToHit,
        this->playerBulletsToRemove.objectsToHit,
        Dod::BufferUtils::createImFromBuffer(this->renderContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->renderContext.yCoords),
        Dod::BufferUtils::createImFromBuffer(enemiesX),
        Dod::BufferUtils::createImFromBuffer(enemiesY),
        this->enemyDimentions.width,
        this->enemyDimentions.height
    );

    Game::Core::Bullets::updateLifetime(
        this->playerBulletsToRemove.objectsToHit,
        this->renderContext.xCoords,
        this->renderContext.yCoords
    );

    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).modelId = 2;
    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).numOfElements = Dod::BufferUtils::getNumFilledElements(this->renderContext.xCoords);

}

void Game::ExecutionBlock::PlayerBullets::flushSharedLocalContexts()
{
    this->soundsContext.reset();
    this->obstaclesToHitContext.reset();
    this->enemiesToHitContext.reset();
    //    this->renderContext.reset();
}
