#include "ObstaclesExecutor.h"
#include "ObstaclesGameplay.h"
#include "ObstaclesCore.h"

#include <fstream>
#include <iostream>

#include <dod/MemUtils.h>
#include <dod/BufferUtils.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>

void Game::ExecutionBlock::Obstacles::loadContext()
{
    this->renderContext.load();
    this->obstaclesParameters.load();
    this->obstaclesDimentionParameters.load();
    this->obstaclesCoordinates.load();
    this->soundsContext.load();
    this->obstaclesToRemove.load();
}

void Game::ExecutionBlock::Obstacles::initiate()
{

    Game::Gameplay::Obstacles::create(
        this->obstaclesCoordinates.xCoords,
        this->obstaclesCoordinates.yCoords,
        this->obstaclesParameters.lifes,
        this->obstaclesParameters.obstaclesPerRow,
        this->obstaclesParameters.obstaclesPerCol,
        this->obstaclesParameters.obstaclesStride,
        this->obstaclesParameters.obstaclesClusters,
        this->obstaclesParameters.obstaclesClustersTotalAreaWidth,
        this->obstaclesParameters.obstaclesClusterY,
        this->obstaclesParameters.initialLifes,
        this->obstaclesParameters.width
    );

}

void Game::ExecutionBlock::Obstacles::update(float dt)
{

    const auto obstaclesToHit{ Dod::SharedContext::get(this->obstaclesToHit).objectsToHit };

    Game::Core::Obstacles::updateLifetime(
        this->obstaclesToRemove.objectsToHit,
        this->obstaclesParameters.lifes,
        Dod::BufferUtils::createImFromBuffer(obstaclesToHit)
    );

    Game::Core::Obstacles::remove(
        this->obstaclesToRemove.objectsToHit,
        this->obstaclesParameters.lifes,
        this->obstaclesCoordinates.xCoords,
        this->obstaclesCoordinates.yCoords
    );

    Dod::BufferUtils::constructBack(this->renderContext.modelsMeta);
    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).modelId = 3;
    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).numOfElements = 
        Dod::BufferUtils::getNumFilledElements(this->obstaclesCoordinates.xCoords);

    Dod::BufferUtils::append(this->renderContext.xCoords, Dod::BufferUtils::createImFromBuffer(this->obstaclesCoordinates.xCoords));
    Dod::BufferUtils::append(this->renderContext.yCoords, Dod::BufferUtils::createImFromBuffer(this->obstaclesCoordinates.yCoords));

}

void Game::ExecutionBlock::Obstacles::flushSharedLocalContexts()
{
    this->soundsContext.reset();
    this->renderContext.reset();
    this->obstaclesToRemove.reset();
}
