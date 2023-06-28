#include "EnemiesBulletsExecutor.h"
#include "BulletsGameplay.h"
#include "BulletsCore.h"
#include "CollisionCore.h"

#include <fstream>
#include <iostream>

#include <dod/MemUtils.h>
#include <dod/BufferUtils.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>

void Game::ExecutionBlock::EnemiesBullets::loadContext()
{

    this->renderContext.load();
    this->soundsContext.load();
    this->obstaclesToHitContext.load();
    this->playerToHit.load();
    this->obstacleParameters.load();
    this->bulletsToRemove.load();
    this->sceneParameters.load();
    this->enemyBulletsParameters.load();
    this->playerDimentions.load();

}

void Game::ExecutionBlock::EnemiesBullets::initiate()
{

    Dod::BufferUtils::constructBack(this->renderContext.modelsMeta);

}

void Game::ExecutionBlock::EnemiesBullets::update(float dt)
{

    const auto bulletsToCreateX{ Dod::SharedContext::get(this->bulletsSContext).xCoords };
    const auto bulletsToCreateY{ Dod::SharedContext::get(this->bulletsSContext).yCoords };

    Dod::BufferUtils::populate(this->soundsContext.soundIdsToPlay, 1, 
        Dod::BufferUtils::getNumFilledElements(bulletsToCreateX));

    Dod::BufferUtils::append(this->renderContext.xCoords,
        Dod::BufferUtils::createImFromBuffer(bulletsToCreateX));
    Dod::BufferUtils::append(this->renderContext.yCoords,
        Dod::BufferUtils::createImFromBuffer(bulletsToCreateY));

    Game::Core::Bullets::updateMovement(
        this->renderContext.yCoords,
        this->enemyBulletsParameters.velocity,
        dt
    );

    const auto obstaclesXCorrds{ Dod::SharedContext::get(this->obstaclesCoordinates).xCoords };
    const auto obstaclesYCorrds{ Dod::SharedContext::get(this->obstaclesCoordinates).yCoords };

    const auto obstaclesWidth{ this->obstacleParameters.width };
    const auto obstaclesHeight{ this->obstacleParameters.height };

    Game::Gameplay::Bullets::testWithObstacles(
        this->bulletsToRemove.objectsToHit,
        this->obstaclesToHitContext.objectsToHit,
        Dod::BufferUtils::createImFromBuffer(this->renderContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->renderContext.yCoords),
        Dod::BufferUtils::createImFromBuffer(obstaclesXCorrds),
        Dod::BufferUtils::createImFromBuffer(obstaclesYCorrds),
        obstaclesWidth,
        obstaclesHeight
    );
    for (int32_t id{}; id < Dod::BufferUtils::getNumFilledElements(this->bulletsToRemove.objectsToHit); ++id)
    {
        std::cout << "Enemy bullet to remove id " << Dod::BufferUtils::get(this->bulletsToRemove.objectsToHit, id) << "\n";
    }
    const auto playerX{ Dod::SharedContext::get(this->playerSContext).xCoords };
    const auto playerY{ Dod::SharedContext::get(this->playerSContext).yCoords };
    const auto playerWidth{ this->playerDimentions.width };
    const auto playerHeight{ this->playerDimentions.height };

    Game::Gameplay::Bullets::collisionUpdate(
        this->playerToHit.objectsToHit,
        this->bulletsToRemove.objectsToHit,
        Dod::BufferUtils::createImFromBuffer(this->renderContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->renderContext.yCoords),
        Dod::BufferUtils::createImFromBuffer(playerX),
        Dod::BufferUtils::createImFromBuffer(playerY),
        playerWidth,
        playerHeight
    );

    Game::Core::Collision::pointsPlaneIntersection(
        this->bulletsToRemove.objectsToHit,
        Dod::BufferUtils::createImFromBuffer(this->renderContext.yCoords),
        this->sceneParameters.bottomPlaneY, 
        this->sceneParameters.bottomPlaneDir
    );

    Game::Core::Bullets::updateLifetime(
        this->bulletsToRemove.objectsToHit,
        this->renderContext.xCoords,
        this->renderContext.yCoords
    );

    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).modelId = 2;
    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).numOfElements = Dod::BufferUtils::getNumFilledElements(this->renderContext.xCoords);

}

void Game::ExecutionBlock::EnemiesBullets::flushSharedLocalContexts()
{
    this->soundsContext.reset();
    this->obstaclesToHitContext.reset();
    this->playerToHit.reset();
    this->bulletsToRemove.reset();
    //    this->renderContext.reset();
}
