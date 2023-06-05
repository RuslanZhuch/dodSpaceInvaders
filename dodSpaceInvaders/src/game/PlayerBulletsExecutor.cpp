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

template <>
const Game::Context::Sounds::Shared& Game::ExecutionBlock::PlayerBullets::getSharedLocalContext<Game::Context::Sounds::Shared>()
{
    return this->soundsContext;
}
template <>
const Game::Context::Render::Shared& Game::ExecutionBlock::PlayerBullets::getSharedLocalContext<Game::Context::Render::Shared>()
{
    return this->renderContext;
}
template <>
const Game::Context::ObjectsToHit::Shared& Game::ExecutionBlock::PlayerBullets::getSharedLocalContext<Game::Context::ObjectsToHit::Shared>()
{
    return this->obstaclesToHitContext;
}

void Game::ExecutionBlock::PlayerBullets::loadContext()
{

    int32_t header{ 0 };
    this->memory.allocate(2048);

    {
        std::ifstream contextFile("resources/contexts/sceneContext.json");
        assert(contextFile.is_open());
        const std::string fileRawData((std::istreambuf_iterator<char>(contextFile)), std::istreambuf_iterator<char>());

        rapidjson::Document json;
        json.Parse(fileRawData.c_str());
        const auto root{ json.GetObject() };

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
                    if (dataElement.name == "sceneParameters" && dataElement.value.IsObject())
                    {
                        const auto sceneParametersObj{ dataElement.value.GetObject() };
                        const auto topPlaneY{ sceneParametersObj["topPlaneY"].GetFloat() };
                        this->sceneParameters.topPlaneY = topPlaneY;
                        const auto topPlaneDir{ sceneParametersObj["topPlaneDire"].GetFloat() };
                        this->sceneParameters.topPlaneDir = topPlaneDir;
                        const auto bottomPlaneY{ sceneParametersObj["bottomPlaneY"].GetFloat() };
                        this->sceneParameters.bottomPlaneY = bottomPlaneY;
                        const auto bottomPlaneDir{ sceneParametersObj["bottomPlaneDir"].GetFloat() };
                        this->sceneParameters.bottomPlaneDir = bottomPlaneDir;
                    }
                }
            }
        }
    }

    {
        std::ifstream contextFile("resources/contexts/playerBulletsContext.json");
        assert(contextFile.is_open());
        const std::string fileRawData((std::istreambuf_iterator<char>(contextFile)), std::istreambuf_iterator<char>());

        rapidjson::Document json;
        json.Parse(fileRawData.c_str());
        const auto root{ json.GetObject() };

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
                    if (dataElement.name == "bulletsParameters" && dataElement.value.IsObject())
                    {
                        const auto enemyBulletsParametersObj{ dataElement.value.GetObject() };
                        [[maybe_unused]] const auto velocity{ enemyBulletsParametersObj["velocity"].GetFloat() };
                        this->playerBulletsParameters.velocity = velocity;
                    }
                    else if (dataElement.name == "bulletsContext" && dataElement.value.IsObject())
                    {
                        const auto enemyBulletsContextObj{ dataElement.value.GetObject() };

                        //                        const auto xCoords{ enemyBulletsContextObj["xCoords"].GetObject() };
                        //                        const auto xCoordsType{ xCoords["type"].GetString() };
                        //                        const auto xCoordsDataType{ xCoords["dataType"].GetString() };
                        //                        const auto xCoordsCapacity{ xCoords["capacity"].GetInt() };
                        //                        const auto xCoordsCapacityBytes{ xCoordsCapacity * sizeof(float) };
                        //                        Dod::BufferUtils::initFromMemory(this->enemyBulletsContext.xCoords, Dod::MemUtils::stackAquire(this->memory, xCoordsCapacityBytes, header));
                        //
                        //                        const auto yCoords{ enemyBulletsContextObj["yCoords"].GetObject() };
                        //                        const auto yCoordsType{ yCoords["type"].GetString() };
                        //                        const auto yCoordsDataType{ yCoords["dataType"].GetString() };
                        //                        const auto yCoordsCapacity{ yCoords["capacity"].GetInt() };
                        //                        const auto yCoordsCapacityBytes{ yCoordsCapacity * sizeof(float) };
                        //                        Dod::BufferUtils::initFromMemory(this->enemyBulletsContext.yCoords, Dod::MemUtils::stackAquire(this->memory, yCoordsCapacityBytes, header));

                        const auto toRemove{ enemyBulletsContextObj["toRemove"].GetObject() };
                        const auto toRemoveType{ toRemove["type"].GetString() };
                        const auto toRemoveDataType{ toRemove["dataType"].GetString() };
                        const auto toRemoveCapacity{ toRemove["capacity"].GetInt() };
                        const auto toRemoveCapacityBytes{ toRemoveCapacity * sizeof(int32_t) };
                        Dod::BufferUtils::initFromMemory(this->playerBulletsContext.toRemove, Dod::MemUtils::stackAquire(this->memory, toRemoveCapacityBytes, header));
                    }
                }
            }
        }
    }

}

void Game::ExecutionBlock::PlayerBullets::initiate()
{

    this->renderContext.init();
    Dod::BufferUtils::constructBack(this->renderContext.modelsMeta);

    this->soundsContext.init();

    this->obstaclesToHitContext.init();

    this->enemiesToHitContext.init();

}

bool Game::ExecutionBlock::PlayerBullets::update(float dt)
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
        this->playerBulletsParameters.velocity,
        dt
    );

    const auto obstaclesXCorrds{ Dod::SharedContext::get(this->obstaclesSContext).xCoords };
    const auto obstaclesYCorrds{ Dod::SharedContext::get(this->obstaclesSContext).yCoords };
    const auto obstaclesWidth{ Dod::SharedContext::get(this->obstaclesSContext).width };
    const auto obstaclesHeight{ Dod::SharedContext::get(this->obstaclesSContext).height };

    Game::Gameplay::Bullets::testWithObstacles(
        this->playerBulletsContext.toRemove,
        this->obstaclesToHitContext.objectsToHit,
        Dod::BufferUtils::createImFromBuffer(this->renderContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->renderContext.yCoords),
        Dod::BufferUtils::createImFromBuffer(obstaclesXCorrds),
        Dod::BufferUtils::createImFromBuffer(obstaclesYCorrds),
        obstaclesWidth,
        obstaclesHeight
    );

    Game::Core::Collision::pointsPlaneIntersection(
        this->playerBulletsContext.toRemove,
        Dod::BufferUtils::createImFromBuffer(this->renderContext.yCoords),
        this->sceneParameters.topPlaneY,
        this->sceneParameters.topPlaneDir
    );

    const auto enemiesX{ Dod::SharedContext::get(this->enemiesSContext).xCoords };
    const auto enemiesY{ Dod::SharedContext::get(this->enemiesSContext).yCoords };
    const auto enemyWidth{ Dod::SharedContext::get(this->enemiesSContext).width };
    const auto enemyHeight{ Dod::SharedContext::get(this->enemiesSContext).height };

    Game::Gameplay::Bullets::collisionUpdate(
        this->enemiesToHitContext.objectsToHit,
        this->playerBulletsContext.toRemove,
        Dod::BufferUtils::createImFromBuffer(this->renderContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->renderContext.yCoords),
        Dod::BufferUtils::createImFromBuffer(enemiesX),
        Dod::BufferUtils::createImFromBuffer(enemiesY),
        enemyWidth,
        enemyHeight
    );

    Game::Core::Bullets::updateLifetime(
        this->playerBulletsContext.toRemove,
        this->renderContext.xCoords,
        this->renderContext.yCoords
    );

    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).modelId = 2;
    Dod::BufferUtils::get(this->renderContext.modelsMeta, 0).numOfElements = Dod::BufferUtils::getNumFilledElements(this->renderContext.xCoords);

    return true;

}

void Game::ExecutionBlock::PlayerBullets::flushSharedLocalContexts()
{
    this->soundsContext.reset();
    this->obstaclesToHitContext.reset();
    this->enemiesToHitContext.reset();
    //    this->renderContext.reset();
}
