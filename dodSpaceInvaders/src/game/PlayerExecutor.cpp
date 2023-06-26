#include "PlayerExecutor.h"
#include "PlayerGameplay.h"

#include <fstream>
#include <iostream>

#include <dod/MemUtils.h>
#include <dod/BufferUtils.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>

template <>
const Game::Context::Sounds::Shared& Game::ExecutionBlock::Player::getSharedLocalContext<Game::Context::Sounds::Shared>()
{
    return this->soundsContext;
}
template <>
const Game::Context::Render::Shared& Game::ExecutionBlock::Player::getSharedLocalContext<Game::Context::Render::Shared>()
{
    return this->renderContext;
}
template <>
const Game::Context::BulletsToSpawn::Shared& Game::ExecutionBlock::Player::getSharedLocalContext<Game::Context::BulletsToSpawn::Shared>()
{
    return this->bulletsToSpawnContext;
}
template <>
const Game::Context::Units::Shared& Game::ExecutionBlock::Player::getSharedLocalContext<Game::Context::Units::Shared>()
{
    return this->unitContext;
}

void Game::ExecutionBlock::Player::loadContext()
{

    this->playerContext.load();

}

void Game::ExecutionBlock::Player::initiate()
{

    this->renderContext.load();
    this->bulletsToSpawnContext.load();

    this->unitContext.load();

    Dod::BufferUtils::constructBack(this->unitContext.xCoords, this->playerContext.xCoord);
    Dod::BufferUtils::constructBack(this->unitContext.yCoords, this->playerContext.yCoord);
    this->unitContext.width = this->playerContext.width;
    this->unitContext.height = this->playerContext.height;

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

    Dod::BufferUtils::constructBack(this->renderContext.modelsMeta, Context::Render::Shared::ModelMeta(4, 1), this->playerContext.lifes > 0);

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
