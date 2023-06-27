#include "PlayerExecutor.h"
#include "PlayerGameplay.h"

#include <fstream>
#include <iostream>

#include <dod/MemUtils.h>
#include <dod/BufferUtils.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>

void Game::ExecutionBlock::Player::loadContext()
{

    this->playerContext.load();

    this->renderContext.load();
    this->bulletsToSpawnContext.load();

    this->unitContext.load();

}

void Game::ExecutionBlock::Player::initiate()
{

    Dod::BufferUtils::constructBack(this->unitContext.xCoords, this->playerContext.xCoord);
    Dod::BufferUtils::constructBack(this->unitContext.yCoords, this->playerContext.yCoord);
    Dod::BufferUtils::populate(this->unitContext.groupWidth, this->playerContext.width, true);
    Dod::BufferUtils::populate(this->unitContext.groupHeight, this->playerContext.height, true);
    Dod::BufferUtils::populate(this->unitContext.elementsInGroup, 1, true);

}

void Game::ExecutionBlock::Player::update(float dt)
{

    const auto toRemove{ Dod::SharedContext::get(this->toHitSContext).objectsToHit };

    Game::Gameplay::Player::lifetimeUpdate(
        Dod::BufferUtils::createImFromBuffer(toRemove),
        this->playerContext.lifes
    );

    Game::Gameplay::Player::updateInputs(
        this->playerContext.inputs,
        this->playerContext.prevInputs
    );

    auto& currX{ Dod::BufferUtils::get(this->unitContext.xCoords, 0) };
    auto& currY{ Dod::BufferUtils::get(this->unitContext.yCoords, 0) };

    Game::Gameplay::Player::updateMovement(
        this->playerContext.move,
        currX,
        this->playerContext.inputs,
        this->playerContext.prevInputs,
        dt
    );

    Dod::BufferUtils::constructBack(this->renderContext.modelsMeta, Context::RenderCommands::Data::ModelMeta(4, 1), this->playerContext.lifes > 0);

    Dod::BufferUtils::constructBack(this->renderContext.xCoords, currX, this->playerContext.lifes > 0);
    Dod::BufferUtils::constructBack(this->renderContext.yCoords, currY, this->playerContext.lifes > 0);

    const auto numOfPlayerBulletsToCreate{ Game::Gameplay::Player::updateFireComponent(
        this->playerContext.lifes,
        this->playerContext.inputs,
        this->playerContext.prevInputs
    ) };

    Game::Gameplay::Player::createBullets(
        numOfPlayerBulletsToCreate,
        this->bulletsToSpawnContext.xCoords,
        this->bulletsToSpawnContext.yCoords,
        currX,
        currY
    );

}

void Game::ExecutionBlock::Player::flushSharedLocalContexts()
{
    this->soundsContext.reset();
    this->bulletsToSpawnContext.reset();
    this->renderContext.reset();
}
