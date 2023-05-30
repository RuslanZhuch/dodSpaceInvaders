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

void Game::ExecutionBlock::Enemies::loadContext()
{

    std::ifstream contextFile("resources/contexts/enemiesContext.json");
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
                if (dataElement.name == "enemiesParameters" && dataElement.value.IsObject())
                {
                    const auto enemiesParametersObj{ dataElement.value.GetObject() };
                    const auto numOfEnemiesPerRow{ enemiesParametersObj["numOfEnemiesPerRow"].GetInt() };
                    this->enemiesParameters.numOfEnemiesPerRow = numOfEnemiesPerRow;
                    const auto numOfEnemiesPerCol{ enemiesParametersObj["numOfEnemiesPerCol"].GetInt() };
                    this->enemiesParameters.numOfEnemiesCols = numOfEnemiesPerCol;
                    const auto enemiesXStride{ enemiesParametersObj["enemiesXStride"].GetFloat() };
                    this->enemiesParameters.enemiesXStride = enemiesXStride;
                    const auto enemiesYStride{ enemiesParametersObj["enemiesYStride"].GetFloat() };
                    this->enemiesParameters.enemiesYStride = enemiesYStride;
                    const auto width{ enemiesParametersObj["width"].GetFloat() };
                    this->enemiesParameters.width = width;
                    const auto height{ enemiesParametersObj["height"].GetFloat() };
                    this->enemiesParameters.height = height;
                    const auto weaponCooldownTime{ enemiesParametersObj["weaponCooldownTime"].GetFloat() };
                    this->enemiesParameters.weaponCooldownTime = weaponCooldownTime;
                    const auto startCoordX{ enemiesParametersObj["startCoordX"].GetFloat() };
                    this->enemyBatchContext.batchCoordX = startCoordX;
                    const auto startCoordY{ enemiesParametersObj["startCoordY"].GetFloat() };
                    this->enemyBatchContext.batchCoordY = startCoordY;
                }
                else if (dataElement.name == "enemiesWeaponContext" && dataElement.value.IsObject())
                {
                    const auto enemiesWeaponContextObj{ dataElement.value.GetObject() };
                    this->enemyWeaponContext.enemyWeaponCooldownTimeLeft = enemiesWeaponContextObj["cooldownTimeLeft"].GetInt();
                }
                else if (dataElement.name == "enemiesUnitsContext" && dataElement.value.IsObject())
                {
                    const auto enemiesUnitsContextObj{ dataElement.value.GetObject() };

//                    const auto xCoords{ enemiesUnitsContextObj["xCoords"].GetObject() };
//                    const auto xCoordsType{ xCoords["type"].GetString() };
//                    const auto xCoordsDataType{ xCoords["dataType"].GetString() };
//                    const auto xCoordsCapacity{ xCoords["capacity"].GetInt() };
//                    const auto xCoordsCapacityBytes{ xCoordsCapacity * sizeof(float) };
//                    Dod::BufferUtils::initFromMemory(this->enemyUnitsContext.xCoords, Dod::MemUtils::stackAquire(this->memory, xCoordsCapacityBytes, header));
//
//                    const auto yCoords{ enemiesUnitsContextObj["yCoords"].GetObject() };
//                    const auto yCoordsType{ yCoords["type"].GetString() };
//                    const auto yCoordsDataType{ yCoords["dataType"].GetString() };
//                    const auto yCoordsCapacity{ yCoords["capacity"].GetInt() };
//                    const auto yCoordsCapacityBytes{ yCoordsCapacity * sizeof(float) };
//                    Dod::BufferUtils::initFromMemory(this->enemyUnitsContext.yCoords, Dod::MemUtils::stackAquire(this->memory, yCoordsCapacityBytes, header));

                    const auto toRemove{ enemiesUnitsContextObj["toRemove"].GetObject() };
                    const auto toRemoveType{ toRemove["type"].GetString() };
                    const auto toRemoveDataType{ toRemove["dataType"].GetString() };
                    const auto toRemoveCapacity{ toRemove["capacity"].GetInt() };
                    const auto toRemoveCapacityBytes{ toRemoveCapacity * sizeof(int32_t) };
                    Dod::BufferUtils::initFromMemory(this->enemyUnitsContext.toRemove, Dod::MemUtils::stackAquire(this->memory, toRemoveCapacityBytes, header));
                }
            }
        }
    }

}

void Game::ExecutionBlock::Enemies::initiate()
{

    this->renderContext.init();
    Dod::BufferUtils::constructBack(this->renderContext.modelsMeta);

    this->bulletsToSpawnContext.init();

    Game::Gameplay::Enemies::generateEnemies(
        this->enemiesParameters.numOfEnemiesPerRow,
        this->enemiesParameters.numOfEnemiesCols,
        this->enemiesParameters.enemiesXStride,
        this->enemiesParameters.enemiesYStride,
        this->enemyBatchContext.batchCoordX,
        this->enemyBatchContext.batchCoordY,
        this->renderContext.xCoords,
        this->renderContext.yCoords
    );

}

bool Game::ExecutionBlock::Enemies::update(float dt)
{

    Game::Gameplay::Enemies::enemiesLifetimeUpdate(
        this->enemyUnitsContext.toRemove,
        this->renderContext.xCoords,
        this->renderContext.yCoords
    );

    Game::Gameplay::Enemies::enemiesUpdate(
        dt,
        this->enemyBatchContext.batchMoveTimeleft,
        this->enemyBatchContext.direction,
        this->enemyBatchContext.batchTargetX,
        this->enemyBatchContext.batchCoordX,
        this->enemyBatchContext.batchCoordY,
        this->renderContext.xCoords,
        this->renderContext.yCoords
    );

    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).modelId = 1;
    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).numOfElements = Dod::BufferUtils::getNumFilledElements(this->renderContext.xCoords);

    const auto numOfEnemyBulletsToCreate{ Game::Gameplay::Enemies::updateEnemyBulletsCreation(
        dt,
        this->enemyWeaponContext.enemyWeaponCooldownTimeLeft,
        Dod::BufferUtils::getNumFilledElements(this->renderContext.xCoords)
    ) };

    Game::Gameplay::Enemies::generateEnemyBullets(
        numOfEnemyBulletsToCreate,
        this->bulletsToSpawnContext.xCoords,
        this->bulletsToSpawnContext.yCoords,
        this->enemyWeaponContext.rand,
        Dod::BufferUtils::createImFromBuffer(this->renderContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->renderContext.yCoords)
    );

	return true;

}

void Game::ExecutionBlock::Enemies::flushSharedLocalContexts()
{
    this->soundsContext.reset();
    this->bulletsToSpawnContext.reset();
//    this->renderContext.reset();
}
