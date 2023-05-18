#include "MainExecution.h"

#include "EnemiesGameplay.h"
#include "BulletsCore.h"
#include "BulletsGameplay.h"
#include "CollisionCore.h"
#include "ObstaclesCore.h"
#include "ObstaclesGameplay.h"
#include "PlayerGameplay.h"

#include "GameRender.h"

#include <dod/MemUtils.h>
#include <dod/BufferUtils.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>

#include <iostream>
#include <fstream>

#undef GetObject;

template <>
const Game::Context::Sounds::Shared& Game::ExecutionBlock::Main::getSharedLocalContext<Game::Context::Sounds::Shared>()
{
    return this->soundsContext;
}

void Game::ExecutionBlock::Main::loadContext()
{

    std::ifstream contextFile("resources/contexts/mainContext.json");
    assert(contextFile.is_open());
    const std::string fileRawData((std::istreambuf_iterator<char>(contextFile)), std::istreambuf_iterator<char>());

    rapidjson::Document json;
    json.Parse(fileRawData.c_str());
    const auto root{ json.GetObject() };

    int32_t header{ 0 };

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
                if (dataElement.name == "applicationContext" && dataElement.value.IsObject())
                {
                    const auto applicationContextObj{ dataElement.value.GetObject() };
                    const auto totalMemoryBytes{ applicationContextObj["totalMemoryBytes"].GetInt() };
                    this->memory.allocate(totalMemoryBytes);
                    const auto windowWidth{ applicationContextObj["windowWidth"].GetInt() };
                    this->commonContext.width = windowWidth;
                    const auto windowHeight{ applicationContextObj["windowHeight"].GetInt() };
                    this->commonContext.height = windowHeight;
                    const auto title{ applicationContextObj["title"].GetString() };
                    const auto titleSize{ applicationContextObj["title"].GetStringLength() };
                    std::memcpy(this->commonContext.title.data(), title, std::min(this->commonContext.title.size(), static_cast<size_t>(titleSize)));
                }
                else if (dataElement.name == "sceneParameters" && dataElement.value.IsObject())
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
                else if (dataElement.name == "enemiesParameters" && dataElement.value.IsObject())
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
                    [[maybe_unused]] const auto cooldownTimeLeft{ enemiesWeaponContextObj["cooldownTimeLeft"].GetInt() };
                }
                else if (dataElement.name == "enemiesUnitsContext" && dataElement.value.IsObject())
                {
                    const auto enemiesUnitsContextObj{ dataElement.value.GetObject() };

                    const auto xCoords{ enemiesUnitsContextObj["xCoords"].GetObject() };
                    const auto xCoordsType{ xCoords["type"].GetString() };
                    const auto xCoordsDataType{ xCoords["dataType"].GetString() };
                    const auto xCoordsCapacity{ xCoords["capacity"].GetInt() };
                    const auto xCoordsCapacityBytes{ xCoordsCapacity * sizeof(float) };
                    Dod::BufferUtils::initFromMemory(this->enemyUnitsContext.xCoords, Dod::MemUtils::stackAquire(this->memory, xCoordsCapacityBytes, header));

                    const auto yCoords{ enemiesUnitsContextObj["yCoords"].GetObject() };
                    const auto yCoordsType{ yCoords["type"].GetString() };
                    const auto yCoordsDataType{ yCoords["dataType"].GetString() };
                    const auto yCoordsCapacity{ yCoords["capacity"].GetInt() };
                    const auto yCoordsCapacityBytes{ yCoordsCapacity * sizeof(float) };
                    Dod::BufferUtils::initFromMemory(this->enemyUnitsContext.yCoords, Dod::MemUtils::stackAquire(this->memory, yCoordsCapacityBytes, header));

                    const auto toRemove{ enemiesUnitsContextObj["toRemove"].GetObject() };
                    const auto toRemoveType{ toRemove["type"].GetString() };
                    const auto toRemoveDataType{ toRemove["dataType"].GetString() };
                    const auto toRemoveCapacity{ toRemove["capacity"].GetInt() };
                    const auto toRemoveCapacityBytes{ toRemoveCapacity * sizeof(int32_t)};
                    Dod::BufferUtils::initFromMemory(this->enemyUnitsContext.toRemove, Dod::MemUtils::stackAquire(this->memory, toRemoveCapacityBytes, header));
                }
                else if (dataElement.name == "enemyBulletsParameters" && dataElement.value.IsObject())
                {
                    const auto enemyBulletsParametersObj{ dataElement.value.GetObject() };
                    [[maybe_unused]] const auto velocity{ enemyBulletsParametersObj["velocity"].GetFloat() };
                    this->enemyBulletsParameters.velocity = velocity;
                }
                else if (dataElement.name == "enemyBulletsContext" && dataElement.value.IsObject())
                {
                    const auto enemyBulletsContextObj{ dataElement.value.GetObject() };

                    const auto xCoords{ enemyBulletsContextObj["xCoords"].GetObject() };
                    const auto xCoordsType{ xCoords["type"].GetString() };
                    const auto xCoordsDataType{ xCoords["dataType"].GetString() };
                    const auto xCoordsCapacity{ xCoords["capacity"].GetInt() };
                    const auto xCoordsCapacityBytes{ xCoordsCapacity * sizeof(float) };
                    Dod::BufferUtils::initFromMemory(this->enemyBulletsContext.xCoords, Dod::MemUtils::stackAquire(this->memory, xCoordsCapacityBytes, header));

                    const auto yCoords{ enemyBulletsContextObj["yCoords"].GetObject() };
                    const auto yCoordsType{ yCoords["type"].GetString() };
                    const auto yCoordsDataType{ yCoords["dataType"].GetString() };
                    const auto yCoordsCapacity{ yCoords["capacity"].GetInt() };
                    const auto yCoordsCapacityBytes{ yCoordsCapacity * sizeof(float) };
                    Dod::BufferUtils::initFromMemory(this->enemyBulletsContext.yCoords, Dod::MemUtils::stackAquire(this->memory, yCoordsCapacityBytes, header));

                    const auto toRemove{ enemyBulletsContextObj["toRemove"].GetObject() };
                    const auto toRemoveType{ toRemove["type"].GetString() };
                    const auto toRemoveDataType{ toRemove["dataType"].GetString() };
                    const auto toRemoveCapacity{ toRemove["capacity"].GetInt() };
                    const auto toRemoveCapacityBytes{ toRemoveCapacity * sizeof(int32_t) };
                    Dod::BufferUtils::initFromMemory(this->enemyBulletsContext.toRemove, Dod::MemUtils::stackAquire(this->memory, toRemoveCapacityBytes, header));
                }
                else if (dataElement.name == "playerParameters" && dataElement.value.IsObject())
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
                else if (dataElement.name == "playerBulletsParameters" && dataElement.value.IsObject())
                {
                    const auto playerBulletsParametersObj{ dataElement.value.GetObject() };
                    [[maybe_unused]] const auto velocity{ playerBulletsParametersObj["velocity"].GetFloat() };
                    this->playerBulletsParameters.velocity = velocity;
                }
                else if (dataElement.name == "playerBulletsContext" && dataElement.value.IsObject())
                {
                    const auto playerBulletsContextObj{ dataElement.value.GetObject() };

                    const auto xCoords{ playerBulletsContextObj["xCoords"].GetObject() };
                    const auto xCoordsType{ xCoords["type"].GetString() };
                    const auto xCoordsDataType{ xCoords["dataType"].GetString() };
                    const auto xCoordsCapacity{ xCoords["capacity"].GetInt() };
                    const auto xCoordsCapacityBytes{ xCoordsCapacity * sizeof(float) };
                    Dod::BufferUtils::initFromMemory(this->playerBulletsContext.xCoords, Dod::MemUtils::stackAquire(this->memory, xCoordsCapacityBytes, header));

                    const auto yCoords{ playerBulletsContextObj["yCoords"].GetObject() };
                    const auto yCoordsType{ yCoords["type"].GetString() };
                    const auto yCoordsDataType{ yCoords["dataType"].GetString() };
                    const auto yCoordsCapacity{ yCoords["capacity"].GetInt() };
                    const auto yCoordsCapacityBytes{ yCoordsCapacity * sizeof(float) };
                    Dod::BufferUtils::initFromMemory(this->playerBulletsContext.yCoords, Dod::MemUtils::stackAquire(this->memory, yCoordsCapacityBytes, header));

                    const auto toRemove{ playerBulletsContextObj["toRemove"].GetObject() };
                    const auto toRemoveType{ toRemove["type"].GetString() };
                    const auto toRemoveDataType{ toRemove["dataType"].GetString() };
                    const auto toRemoveCapacity{ toRemove["capacity"].GetInt() };
                    const auto toRemoveCapacityBytes{ toRemoveCapacity * sizeof(int32_t) };
                    Dod::BufferUtils::initFromMemory(this->playerBulletsContext.toRemove, Dod::MemUtils::stackAquire(this->memory, toRemoveCapacityBytes, header));
                }
                else if (dataElement.name == "obstaclesParameters" && dataElement.value.IsObject())
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
                }
                else if (dataElement.name == "obstaclesUnitsContext" && dataElement.value.IsObject())
                {
                    const auto obstaclesUnitsContextObj{ dataElement.value.GetObject() };

                    const auto xCoords{ obstaclesUnitsContextObj["xCoords"].GetObject() };
                    const auto xCoordsType{ xCoords["type"].GetString() };
                    const auto xCoordsDataType{ xCoords["dataType"].GetString() };
                    const auto xCoordsCapacity{ xCoords["capacity"].GetInt() };
                    const auto xCoordsCapacityBytes{ xCoordsCapacity * sizeof(float) };
                    Dod::BufferUtils::initFromMemory(this->obstaclesContext.xCoords, Dod::MemUtils::stackAquire(this->memory, xCoordsCapacityBytes, header));

                    const auto yCoords{ obstaclesUnitsContextObj["yCoords"].GetObject() };
                    const auto yCoordsType{ yCoords["type"].GetString() };
                    const auto yCoordsDataType{ yCoords["dataType"].GetString() };
                    const auto yCoordsCapacity{ yCoords["capacity"].GetInt() };
                    const auto yCoordsCapacityBytes{ yCoordsCapacity * sizeof(float) };
                    Dod::BufferUtils::initFromMemory(this->obstaclesContext.yCoords, Dod::MemUtils::stackAquire(this->memory, yCoordsCapacityBytes, header));

                    const auto lifes{ obstaclesUnitsContextObj["lifes"].GetObject() };
                    const auto lifesType{ lifes["type"].GetString() };
                    const auto lifesDataType{ lifes["dataType"].GetString() };
                    const auto lifesCapacity{ lifes["capacity"].GetInt() };
                    const auto lifesCapacityBytes{ lifesCapacity * sizeof(int32_t) };
                    Dod::BufferUtils::initFromMemory(this->obstaclesContext.lifes, Dod::MemUtils::stackAquire(this->memory, lifesCapacityBytes, header));

                    const auto toHit{ obstaclesUnitsContextObj["toHit"].GetObject() };
                    const auto toHitType{ toHit["type"].GetString() };
                    const auto toHitDataType{ toHit["dataType"].GetString() };
                    const auto toHitCapacity{ toHit["capacity"].GetInt() };
                    const auto toHitCapacityBytes{ toHitCapacity * sizeof(int32_t) };
                    Dod::BufferUtils::initFromMemory(this->obstaclesContext.toHit, Dod::MemUtils::stackAquire(this->memory, toHitCapacityBytes, header));

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

void Game::ExecutionBlock::Main::initiate()
{

    Game::Gameplay::Enemies::generateEnemies(
        this->enemiesParameters.numOfEnemiesPerRow,
        this->enemiesParameters.numOfEnemiesCols,
        this->enemiesParameters.enemiesXStride,
        this->enemiesParameters.enemiesYStride,
        this->enemyBatchContext.batchCoordX,
        this->enemyBatchContext.batchCoordY,
        this->enemyUnitsContext.xCoords,
        this->enemyUnitsContext.yCoords
    );


    Game::Gameplay::Obstacles::create(
        this->obstaclesContext.xCoords,
        this->obstaclesContext.yCoords,
        this->obstaclesContext.lifes,
        this->obstaclesParameters.obstaclesPerRow,
        this->obstaclesParameters.obstaclesPerCol,
        this->obstaclesParameters.obstaclesStride,
        this->obstaclesParameters.obstaclesClusters,
        this->obstaclesParameters.obstaclesClustersTotalAreaWidth,
        this->obstaclesParameters.obstaclesClusterY,
        this->obstaclesParameters.initialLifes,
        this->commonContext.width
    );

    this->gameRenderer = std::make_unique<GameRenderer>(commonContext.width, commonContext.height, commonContext.title.data());

    this->soundsContext.init(); 

}

bool Game::ExecutionBlock::Main::update(float dt)
{

    sf::Event event;

    auto& window{ this->gameRenderer->getWindow() };
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    window.clear();

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

    const auto numOfPlayerBulletsToCreate{ Game::Gameplay::Player::updateFireComponent(
        this->playerLifetimeContext.lifes,
        this->playerInputsContext.inputs,
        this->playerInputsContext.prevInputs
    ) };

    Game::Gameplay::Player::createBulletsSFx(
        this->soundsContext.soundIdsToPlay,
        numOfPlayerBulletsToCreate
    );

    Game::Gameplay::Player::createBullets(
        numOfPlayerBulletsToCreate,
        this->playerBulletsContext.xCoords, 
        this->playerBulletsContext.yCoords,
        this->playerPositionContext.xCoord,
        this->playerPositionContext.yCoord
    );
        
    Game::Core::Bullets::updateMovement(
        this->playerBulletsContext.yCoords, 
        this->playerBulletsParameters.velocity, 
        dt
    );
    Game::Gameplay::Bullets::collisionUpdate(
        this->enemyUnitsContext.toRemove,
        this->playerBulletsContext.toRemove, 
        Dod::BufferUtils::createImFromBuffer(this->playerBulletsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->playerBulletsContext.yCoords),
        Dod::BufferUtils::createImFromBuffer(this->enemyUnitsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->enemyUnitsContext.yCoords),
        this->enemiesParameters.width,
        this->enemiesParameters.height
    );

    Game::Core::Collision::pointsPlaneIntersection(
        this->playerBulletsContext.toRemove,
        Dod::BufferUtils::createImFromBuffer(this->playerBulletsContext.yCoords),
        this->sceneParameters.topPlaneY,
        this->sceneParameters.topPlaneDir
    );

    Game::Gameplay::Bullets::testWithObstacles(
        this->playerBulletsContext.toRemove,
        this->obstaclesContext.toHit,
        Dod::BufferUtils::createImFromBuffer(this->playerBulletsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->playerBulletsContext.yCoords),
        Dod::BufferUtils::createImFromBuffer(this->obstaclesContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->obstaclesContext.yCoords),
        this->obstaclesParameters.obstaclesStride * 0.5f,
        this->obstaclesParameters.obstaclesStride * 0.5f
    );

    if (Dod::BufferUtils::getNumFilledElements(this->obstaclesContext.toHit) > 0)
        std::cout << Dod::BufferUtils::getNumFilledElements(this->obstaclesContext.toHit) << '\n';

    Game::Core::Bullets::updateLifetime(
        this->playerBulletsContext.toRemove,
        this->playerBulletsContext.xCoords,
        this->playerBulletsContext.yCoords
    );

    Game::Gameplay::Enemies::enemiesLifetimeUpdate(
        this->enemyUnitsContext.toRemove,
        this->enemyUnitsContext.xCoords,
        this->enemyUnitsContext.yCoords
    );

    Game::Gameplay::Enemies::enemiesUpdate(
        dt, 
        this->enemyBatchContext.batchMoveTimeleft,
        this->enemyBatchContext.direction,
        this->enemyBatchContext.batchTargetX,
        this->enemyBatchContext.batchCoordX,
        this->enemyBatchContext.batchCoordY,
        this->enemyUnitsContext.xCoords,
        this->enemyUnitsContext.yCoords
    );

    const auto numOfEnemyBulletsToCreate{ Game::Gameplay::Enemies::updateEnemyBulletsCreation(
        dt,
        this->enemyWeaponContext.enemyWeaponCooldownTimeLeft,
        Dod::BufferUtils::getNumFilledElements(this->enemyUnitsContext.xCoords)
    ) };

    Game::Gameplay::Enemies::createBulletsSFx(
        this->soundsContext.soundIdsToPlay,
        numOfEnemyBulletsToCreate
    );

    Game::Gameplay::Enemies::generateEnemyBullets(
        numOfEnemyBulletsToCreate,
        this->enemyBulletsContext.xCoords,
        this->enemyBulletsContext.yCoords,
        this->enemyWeaponContext.rand,
        Dod::BufferUtils::createImFromBuffer(this->enemyUnitsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->enemyUnitsContext.yCoords)
    );

    Game::Core::Bullets::updateMovement(
        this->enemyBulletsContext.yCoords,
        this->enemyBulletsParameters.velocity,
        dt
    );

    Game::Core::Collision::pointsPlaneIntersection(
        this->enemyBulletsContext.toRemove,
        Dod::BufferUtils::createImFromBuffer(this->enemyBulletsContext.yCoords),
        this->sceneParameters.bottomPlaneY, 
        this->sceneParameters.bottomPlaneDir
    );

    Game::Gameplay::Player::testWithBullets(
        this->enemyBulletsContext.toRemove,
        Dod::BufferUtils::createImFromBuffer(this->enemyBulletsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->enemyBulletsContext.yCoords),
        this->playerLifetimeContext.lifes,
        this->playerPositionContext.xCoord,
        this->playerPositionContext.yCoord,
        this->playerParameters.width,
        this->playerParameters.height
    );

    Game::Gameplay::Bullets::testWithObstacles(
        this->enemyBulletsContext.toRemove,
        this->obstaclesContext.toHit,
        Dod::BufferUtils::createImFromBuffer(this->enemyBulletsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->enemyBulletsContext.yCoords),
        Dod::BufferUtils::createImFromBuffer(this->obstaclesContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->obstaclesContext.yCoords),
        this->obstaclesParameters.obstaclesStride * 0.5f,
        this->obstaclesParameters.obstaclesStride * 0.5f
    );

    Game::Core::Bullets::updateLifetime(
        this->enemyBulletsContext.toRemove,
        this->enemyBulletsContext.xCoords,
        this->enemyBulletsContext.yCoords
    );

    Game::Core::Obstacles::updateLifetime(
        this->obstaclesContext.toRemove,
        this->obstaclesContext.toHit,
        this->obstaclesContext.lifes
    );

    Game::Core::Obstacles::remove(
        this->obstaclesContext.toRemove,
        this->obstaclesContext.lifes,
        this->obstaclesContext.xCoords,
        this->obstaclesContext.yCoords
    );

    Game::SceneRenderer::drawEnemies(
        *this->gameRenderer, 
        Dod::BufferUtils::createImFromBuffer(this->enemyUnitsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->enemyUnitsContext.yCoords)
    );

    Game::SceneRenderer::drawObstacles(
        *this->gameRenderer, 
        Dod::BufferUtils::createImFromBuffer(this->obstaclesContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->obstaclesContext.yCoords)
    );

    Game::SceneRenderer::drawField(*this->gameRenderer);
    Game::SceneRenderer::drawPlayer(
        *this->gameRenderer, 
        { this->playerPositionContext.xCoord, this->playerPositionContext.yCoord },
        this->playerParameters.width,
        this->playerParameters.height,
        this->playerLifetimeContext.lifes > 0
    );

    Game::SceneRenderer::drawBullets(
        *this->gameRenderer,
        Dod::BufferUtils::createImFromBuffer(this->playerBulletsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->playerBulletsContext.yCoords)
    );

    Game::SceneRenderer::drawBullets(
        *this->gameRenderer,
        Dod::BufferUtils::createImFromBuffer(this->enemyBulletsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->enemyBulletsContext.yCoords)
    );

    window.display();

    return window.isOpen();

}

void Game::ExecutionBlock::Main::flushSharedLocalContexts()
{
    this->soundsContext.reset();
}
