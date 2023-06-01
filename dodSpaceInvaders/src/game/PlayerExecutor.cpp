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
                    this->playerPositionContext = { positionX, positionY };
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

}

bool Game::ExecutionBlock::Player::update(float dt)
{
    
    Game::Gameplay::Player::updateInputs(
        this->playerInputsContext.inputs,
        this->playerInputsContext.prevInputs
    );

    Game::Gameplay::Player::updateMovement(
        this->playerMovementContext.move,
        this->playerPositionContext.xCoord,
        this->playerInputsContext.inputs,
        this->playerInputsContext.prevInputs,
        dt
    );

    Dod::BufferUtils::constructBack(this->renderContext.modelsMeta);
    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).modelId = 4;
    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).numOfElements = 1;

    Dod::BufferUtils::constructBack(this->renderContext.xCoords, this->playerPositionContext.xCoord);
    Dod::BufferUtils::constructBack(this->renderContext.yCoords, this->playerPositionContext.yCoord);

//    const auto numOfEnemyBulletsToCreate{ Game::Gameplay::Player::updateEnemyBulletsCreation(
//        dt,
//        this->enemyWeaponContext.enemyWeaponCooldownTimeLeft,
//        Dod::BufferUtils::getNumFilledElements(this->renderContext.xCoords)
//    ) };
//
//    Game::Gameplay::Player::generateEnemyBullets(
//        numOfEnemyBulletsToCreate,
//        this->bulletsToSpawnContext.xCoords,
//        this->bulletsToSpawnContext.yCoords,
//        this->enemyWeaponContext.rand,
//        Dod::BufferUtils::createImFromBuffer(this->renderContext.xCoords),
//        Dod::BufferUtils::createImFromBuffer(this->renderContext.yCoords)
//    );

    return true;

}

void Game::ExecutionBlock::Player::flushSharedLocalContexts()
{
    this->soundsContext.reset();
    this->bulletsToSpawnContext.reset();
    this->renderContext.reset();
}
