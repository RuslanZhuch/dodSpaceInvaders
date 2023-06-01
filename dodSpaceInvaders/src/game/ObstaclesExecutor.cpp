#include "ObstaclesExecutor.h"
#include "ObstaclesGameplay.h"
#include "ObstaclesCore.h"

#include <fstream>
#include <iostream>

#include <dod/MemUtils.h>
#include <dod/BufferUtils.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>

template <>
const Game::Context::Render::Shared& Game::ExecutionBlock::Obstacles::getSharedLocalContext<Game::Context::Render::Shared>()
{
    return this->renderContext;
}
template <>
const Game::Context::Obstacles::Shared& Game::ExecutionBlock::Obstacles::getSharedLocalContext<Game::Context::Obstacles::Shared>()
{
    return this->obstaclesSContext;
}

void Game::ExecutionBlock::Obstacles::loadContext()
{

    std::ifstream contextFile("resources/contexts/obstaclesContext.json");
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

                if (dataElement.name == "obstaclesParameters" && dataElement.value.IsObject())
                {
                    const auto obstaclesParametersObj{ dataElement.value.GetObject() };
                    const auto stride{ obstaclesParametersObj["stride"].GetFloat() };
                    this->obstaclesParameters.obstaclesStride = stride;
                    const auto perRow{ obstaclesParametersObj["perRow"].GetInt() };
                    this->obstaclesParameters.obstaclesPerRow = perRow;
                    const auto perCol{ obstaclesParametersObj["perCol"].GetInt() };
                    this->obstaclesParameters.obstaclesPerCol = perCol;
                    const auto numOfClusters{ obstaclesParametersObj["numOfClusters"].GetInt() };
                    this->obstaclesParameters.obstaclesClusters = numOfClusters;
                    const auto clustersTotalAreaWidth{ obstaclesParametersObj["clustersTotalAreaWidth"].GetInt() };
                    this->obstaclesParameters.obstaclesClustersTotalAreaWidth = clustersTotalAreaWidth;
                    const auto clusterY{ obstaclesParametersObj["clusterY"].GetInt() };
                    this->obstaclesParameters.obstaclesClusterY = clusterY;
                    const auto initialLifes{ obstaclesParametersObj["initialLifes"].GetInt() };
                    this->obstaclesParameters.initialLifes = initialLifes;
                    const auto width{ obstaclesParametersObj["width"].GetFloat() };
                    this->obstaclesParameters.width = width;
                }
                else if (dataElement.name == "obstaclesUnitsContext" && dataElement.value.IsObject())
                {
                    const auto obstaclesUnitsContextObj{ dataElement.value.GetObject() };

//                    const auto xCoords{ obstaclesUnitsContextObj["xCoords"].GetObject() };
//                    const auto xCoordsType{ xCoords["type"].GetString() };
//                    const auto xCoordsDataType{ xCoords["dataType"].GetString() };
//                    const auto xCoordsCapacity{ xCoords["capacity"].GetInt() };
//                    const auto xCoordsCapacityBytes{ xCoordsCapacity * sizeof(float) };
//                    Dod::BufferUtils::initFromMemory(this->obstaclesContext.xCoords, Dod::MemUtils::stackAquire(this->memory, xCoordsCapacityBytes, header));
//
//                    const auto yCoords{ obstaclesUnitsContextObj["yCoords"].GetObject() };
//                    const auto yCoordsType{ yCoords["type"].GetString() };
//                    const auto yCoordsDataType{ yCoords["dataType"].GetString() };
//                    const auto yCoordsCapacity{ yCoords["capacity"].GetInt() };
//                    const auto yCoordsCapacityBytes{ yCoordsCapacity * sizeof(float) };
//                    Dod::BufferUtils::initFromMemory(this->obstaclesContext.yCoords, Dod::MemUtils::stackAquire(this->memory, yCoordsCapacityBytes, header));

                    const auto lifes{ obstaclesUnitsContextObj["lifes"].GetObject() };
                    const auto lifesType{ lifes["type"].GetString() };
                    const auto lifesDataType{ lifes["dataType"].GetString() };
                    const auto lifesCapacity{ lifes["capacity"].GetInt() };
                    const auto lifesCapacityBytes{ lifesCapacity * sizeof(int32_t) };
                    Dod::BufferUtils::initFromMemory(this->obstaclesContext.lifes, Dod::MemUtils::stackAquire(this->memory, lifesCapacityBytes, header));

//                    const auto toHit{ obstaclesUnitsContextObj["toHit"].GetObject() };
//                    const auto toHitType{ toHit["type"].GetString() };
//                    const auto toHitDataType{ toHit["dataType"].GetString() };
//                    const auto toHitCapacity{ toHit["capacity"].GetInt() };
//                    const auto toHitCapacityBytes{ toHitCapacity * sizeof(int32_t) };
//                    Dod::BufferUtils::initFromMemory(this->obstaclesContext.toHit, Dod::MemUtils::stackAquire(this->memory, toHitCapacityBytes, header));

                    const auto toRemove{ obstaclesUnitsContextObj["toRemove"].GetObject() };
                    const auto toRemoveType{ toRemove["type"].GetString() };
                    const auto toRemoveDataType{ toRemove["dataType"].GetString() };
                    const auto toRemoveCapacity{ toRemove["capacity"].GetInt() };
                    const auto toRemoveCapacityBytes{ toRemoveCapacity * sizeof(int32_t) };
                    Dod::BufferUtils::initFromMemory(this->obstaclesContext.toRemove, Dod::MemUtils::stackAquire(this->memory, toRemoveCapacityBytes, header));
                }
            }
        }
    }

}

void Game::ExecutionBlock::Obstacles::initiate()
{

    this->renderContext.init();
    this->obstaclesSContext.init();

    Game::Gameplay::Obstacles::create(
        this->obstaclesSContext.xCoords,
        this->obstaclesSContext.yCoords,
        this->obstaclesContext.lifes,
        this->obstaclesParameters.obstaclesPerRow,
        this->obstaclesParameters.obstaclesPerCol,
        this->obstaclesParameters.obstaclesStride,
        this->obstaclesParameters.obstaclesClusters,
        this->obstaclesParameters.obstaclesClustersTotalAreaWidth,
        this->obstaclesParameters.obstaclesClusterY,
        this->obstaclesParameters.initialLifes,
        this->obstaclesParameters.width
    );

    this->obstaclesSContext.width = 15.f;
    this->obstaclesSContext.height = 15.f;

}

bool Game::ExecutionBlock::Obstacles::update(float dt)
{

    const auto obstaclesToHit{ Dod::SharedContext::get(this->sContext).objectsToHit };

    Game::Core::Obstacles::updateLifetime(
        this->obstaclesContext.toRemove,
        this->obstaclesContext.lifes,
        Dod::BufferUtils::createImFromBuffer(obstaclesToHit)
    );

    Game::Core::Obstacles::remove(
        this->obstaclesContext.toRemove,
        this->obstaclesContext.lifes,
        this->obstaclesSContext.xCoords,
        this->obstaclesSContext.yCoords
    );

    Dod::BufferUtils::constructBack(this->renderContext.modelsMeta);
    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).modelId = 3;
    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).numOfElements = 
        Dod::BufferUtils::getNumFilledElements(this->obstaclesSContext.xCoords);

    Dod::BufferUtils::append(this->renderContext.xCoords, Dod::BufferUtils::createImFromBuffer(this->obstaclesSContext.xCoords));
    Dod::BufferUtils::append(this->renderContext.yCoords, Dod::BufferUtils::createImFromBuffer(this->obstaclesSContext.yCoords));

    return true;

}

void Game::ExecutionBlock::Obstacles::flushSharedLocalContexts()
{
    this->soundsContext.reset();
    this->renderContext.reset();
}
