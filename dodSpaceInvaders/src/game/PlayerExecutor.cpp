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

    std::ifstream contextFile("resources/contexts/playerContext.json");
    assert(contextFile.is_open());
    const std::string fileRawData((std::istreambuf_iterator<char>(contextFile)), std::istreambuf_iterator<char>());

    rapidjson::Document json;
    json.Parse(fileRawData.c_str());
    const auto root{ json.GetObject() };

    int32_t header{ 0 };
    this->memory.allocate(2048);

    for (const auto& element : root)
    {
        if (element.name == "contextName")
        {
            const auto contextName{ element.value.GetString() };
            std::cout << "Loading context: " << contextName << "\n";
        }
        else if (element.name == "data")
        {
            for (const auto& dataElement : element.value.GetObject())
            {
                if (dataElement.name == "playerParameters" && dataElement.value.IsObject())
                {
                    const auto playerParametersObj{ dataElement.value.GetObject() };
                    const auto width{ playerParametersObj["width"].GetFloat() };
                    this->playerParameters.width = width;
                    const auto height{ playerParametersObj["height"].GetFloat() };
                    this->playerParameters.height = height;
                }
                else if (dataElement.name == "playerPositionContext" && dataElement.value.IsObject())
                {
                   const auto playerPositionContextObj{ dataElement.value.GetObject() };
                   const auto positionX{ playerPositionContextObj["positionX"].GetFloat() };
                   const auto positionY{ playerPositionContextObj["positionY"].GetFloat() };
                   this->playerPositionParameters = { positionX, positionY };
                }
                else if (dataElement.name == "playerLifetimeContext" && dataElement.value.IsObject())
                {
                    const auto playerLifetimeContextObj{ dataElement.value.GetObject() };
                    const auto lifes{ playerLifetimeContextObj["lifes"].GetInt() };
                    this->playerLifetimeContext.lifes = lifes;
                }
                else if (dataElement.name == "playerInputsContext" && dataElement.value.IsObject())
                {
                    const auto playerInputsContextObj{ dataElement.value.GetObject() };
                    const auto inputs{ playerInputsContextObj["inputs"].GetInt() };
                    const auto prevInputs{ playerInputsContextObj["prevInputs"].GetInt() };
                }
                else if (dataElement.name == "playerMovement" && dataElement.value.IsObject())
                {
                    const auto playerMovementObj{ dataElement.value.GetObject() };
                    const auto move{ playerMovementObj["move"].GetInt() };
                }
                else if (dataElement.name == "playerFire" && dataElement.value.IsObject())
                {
                    const auto playerFireObj{ dataElement.value.GetObject() };
                    const auto move{ playerFireObj["fire"].GetInt() };
                }
            }
        }
    }

}

void Game::ExecutionBlock::Player::initiate()
{

    this->renderContext.init();
    this->bulletsToSpawnContext.init();

    this->unitContext.init();

    Dod::BufferUtils::constructBack(this->unitContext.xCoords, this->playerPositionParameters.xCoord);
    Dod::BufferUtils::constructBack(this->unitContext.yCoords, this->playerPositionParameters.yCoord);
    this->unitContext.width = this->playerParameters.width;
    this->unitContext.height = this->playerParameters.height;

}

void Game::ExecutionBlock::Player::update(float dt)
{

    const auto toRemove{ Dod::SharedContext::get(this->toHitSContext).objectsToHit };

    Game::Gameplay::Player::lifetimeUpdate(
        Dod::BufferUtils::createImFromBuffer(toRemove),
        this->playerLifetimeContext.lifes
    );

    Game::Gameplay::Player::updateInputs(
        this->playerInputsContext.inputs,
        this->playerInputsContext.prevInputs
    );

    auto& currX{ Dod::BufferUtils::get(this->unitContext.xCoords, 0) };
    auto& currY{ Dod::BufferUtils::get(this->unitContext.yCoords, 0) };

    Game::Gameplay::Player::updateMovement(
        this->playerMovementContext.move,
        currX,
        this->playerInputsContext.inputs,
        this->playerInputsContext.prevInputs,
        dt
    );

    Dod::BufferUtils::constructBack(this->renderContext.modelsMeta, Context::Render::Shared::ModelMeta(4, 1), this->playerLifetimeContext.lifes > 0);

    Dod::BufferUtils::constructBack(this->renderContext.xCoords, currX, this->playerLifetimeContext.lifes > 0);
    Dod::BufferUtils::constructBack(this->renderContext.yCoords, currY, this->playerLifetimeContext.lifes > 0);

    const auto numOfPlayerBulletsToCreate{ Game::Gameplay::Player::updateFireComponent(
        this->playerLifetimeContext.lifes,
        this->playerInputsContext.inputs,
        this->playerInputsContext.prevInputs
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
