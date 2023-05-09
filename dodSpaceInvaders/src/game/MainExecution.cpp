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

#include <iostream>

struct EnemiesParamets
{
    static constexpr auto numOfEnemiesPerRow{ 10 };
    static constexpr auto numOfEnemiesCols{ 4 };
    static constexpr auto enemiesXStride{ 50.f };
    static constexpr auto enemiesYStride{ 50.f };
    static constexpr auto width{ 25.f };
    static constexpr auto height{ 30.f };
};

struct ObstaclesParamets
{
    static constexpr auto obstaclesStride{ 30.f };
    static constexpr auto obstaclesPerRow{ 5 };
    static constexpr auto obstaclesPerCol{ 2 };
    static constexpr auto obstaclesClusters{ 3 };
    static constexpr auto obstaclesClustersTotalAreaWidth{ 700 };
    static constexpr auto obstaclesClusterY{ 650.f };
    static constexpr auto initialLifes{ 1 };
};

static constexpr auto totalBytesForScene{ 1024 * 1024 * 10 };

Game::ExecutionBlock::Main::Main()
    :memory(totalBytesForScene)
{
}

void Game::ExecutionBlock::Main::loadContext()
{

    this->parameters.numOfEnemiesPerRow = EnemiesParamets::numOfEnemiesPerRow;
    this->parameters.numOfEnemiesCols = EnemiesParamets::numOfEnemiesCols;
    this->parameters.enemiesXStride = EnemiesParamets::enemiesXStride;
    this->parameters.enemiesYStride = EnemiesParamets::enemiesYStride;
    this->parameters.width = EnemiesParamets::width;
    this->parameters.height = EnemiesParamets::height;

    this->obstaclesParameters.initialLifes = ObstaclesParamets::initialLifes;
    this->obstaclesParameters.obstaclesClusters = ObstaclesParamets::obstaclesClusters;
    this->obstaclesParameters.obstaclesClustersTotalAreaWidth = ObstaclesParamets::obstaclesClustersTotalAreaWidth;
    this->obstaclesParameters.obstaclesClusterY = ObstaclesParamets::obstaclesClusterY;
    this->obstaclesParameters.obstaclesPerCol = ObstaclesParamets::obstaclesPerCol;
    this->obstaclesParameters.obstaclesPerRow = ObstaclesParamets::obstaclesPerRow;
    this->obstaclesParameters.obstaclesStride = ObstaclesParamets::obstaclesStride;

    int32_t header{ 0 };
    constexpr auto totalEnemies{ EnemiesParamets::numOfEnemiesCols * EnemiesParamets::numOfEnemiesPerRow + 1 };
    constexpr auto enemiesBytesToAquire{ totalEnemies * 4 };
    Dod::BufferUtils::initFromMemory(this->unitsContext.xCoords, Dod::MemUtils::stackAquire(this->memory, enemiesBytesToAquire, header));
    Dod::BufferUtils::initFromMemory(this->unitsContext.yCoords, Dod::MemUtils::stackAquire(this->memory, enemiesBytesToAquire, header));
    Dod::BufferUtils::initFromMemory(this->unitsContext.toRemove, Dod::MemUtils::stackAquire(this->memory, enemiesBytesToAquire, header));

    constexpr auto maxinumEnemyBullets{ 512 };
    constexpr auto enemyBulletsBytesToAquire{ maxinumEnemyBullets * 4 };
    Dod::BufferUtils::initFromMemory(this->enemyBulletsContext.xCoords, Dod::MemUtils::stackAquire(this->memory, enemyBulletsBytesToAquire, header));
    Dod::BufferUtils::initFromMemory(this->enemyBulletsContext.yCoords, Dod::MemUtils::stackAquire(this->memory, enemyBulletsBytesToAquire, header));
    Dod::BufferUtils::initFromMemory(this->enemyBulletsContext.toRemove, Dod::MemUtils::stackAquire(this->memory, 1024, header));


    const auto totalObstacles{ this->obstaclesParameters.obstaclesPerRow * this->obstaclesParameters.obstaclesPerCol * this->obstaclesParameters.obstaclesClusters + 1 };
    const auto obstaclesBytesToAquire{ totalObstacles * 4 };
    Dod::BufferUtils::initFromMemory(this->obstaclesContext.xCoords, Dod::MemUtils::stackAquire(this->memory, obstaclesBytesToAquire, header));
    Dod::BufferUtils::initFromMemory(this->obstaclesContext.yCoords, Dod::MemUtils::stackAquire(this->memory, obstaclesBytesToAquire, header));
    Dod::BufferUtils::initFromMemory(this->obstaclesContext.lifes, Dod::MemUtils::stackAquire(this->memory, obstaclesBytesToAquire, header));
    Dod::BufferUtils::initFromMemory(this->obstaclesContext.toHit, Dod::MemUtils::stackAquire(this->memory, obstaclesBytesToAquire, header));
    Dod::BufferUtils::initFromMemory(this->obstaclesContext.toRemove, Dod::MemUtils::stackAquire(this->memory, obstaclesBytesToAquire, header));


    constexpr auto maxinumPlayerBullets{ 512 };
    constexpr auto playerBulletsBytesToAquire{ maxinumPlayerBullets * 4 };
    Dod::BufferUtils::initFromMemory(this->playerBulletsContext.xCoords, Dod::MemUtils::stackAquire(this->memory, playerBulletsBytesToAquire, header));
    Dod::BufferUtils::initFromMemory(this->playerBulletsContext.yCoords, Dod::MemUtils::stackAquire(this->memory, playerBulletsBytesToAquire, header));
    Dod::BufferUtils::initFromMemory(this->playerBulletsContext.toRemove, Dod::MemUtils::stackAquire(this->memory, 1024, header));

}

void Game::ExecutionBlock::Main::initiate()
{

    this->commonContext.width = 800.f;
    this->commonContext.height = 900.f;

    Game::Gameplay::Enemies::generateEnemies(
        this->parameters.numOfEnemiesPerRow,
        this->parameters.numOfEnemiesCols,
        this->parameters.enemiesXStride,
        this->parameters.enemiesYStride,
        this->batchContext.batchCoordX,
        this->batchContext.batchCoordY,
        this->unitsContext.xCoords,
        this->unitsContext.yCoords
    );

    this->playerPositionContext = { 400.f, 850.f };
    this->playerLifetimeContext.lifes = 1;

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

    this->gameRenderer = std::make_unique<GameRenderer>(commonContext.width, commonContext.height, "dod Space Invaders");

}

void Game::ExecutionBlock::Main::update(float dt)
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

    Game::Gameplay::Player::createBullets(
        this->playerBulletsContext.xCoords, 
        this->playerBulletsContext.yCoords,
        this->playerPositionContext.xCoord,
        this->playerPositionContext.yCoord,
        this->playerLifetimeContext.lifes,
        this->playerInputsContext.inputs,
        this->playerInputsContext.prevInputs
    );
        
    Game::Core::Bullets::updateMovement(this->playerBulletsContext.yCoords, -220.f, dt);
    Game::Gameplay::Bullets::collisionUpdate(
        this->unitsContext.toRemove,
        this->playerBulletsContext.toRemove, 
        Dod::BufferUtils::createImFromBuffer(this->playerBulletsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->playerBulletsContext.yCoords),
        Dod::BufferUtils::createImFromBuffer(this->unitsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->unitsContext.yCoords),
        this->parameters.width,
        this->parameters.height
    );

    Game::Core::Collision::pointsPlaneIntersection(
        this->playerBulletsContext.toRemove,
        Dod::BufferUtils::createImFromBuffer(this->playerBulletsContext.yCoords),
        100.f, 
        1.f
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

    if (this->obstaclesContext.toHit.numOfFilledEls > 0)
        std::cout << this->obstaclesContext.toHit.numOfFilledEls << '\n';

    Game::Core::Bullets::updateLifetime(
        this->playerBulletsContext.toRemove,
        this->playerBulletsContext.xCoords,
        this->playerBulletsContext.yCoords
    );

    Game::Gameplay::Enemies::enemiesLifetimeUpdate(
        this->unitsContext.toRemove,
        this->unitsContext.xCoords,
        this->unitsContext.yCoords
    );

    Game::Gameplay::Enemies::enemiesUpdate(
        dt, 
        this->batchContext.batchMoveTimeleft,
        this->batchContext.direction,
        this->batchContext.batchTargetX,
        this->batchContext.batchCoordX,
        this->batchContext.batchCoordY,
        this->unitsContext.xCoords,
        this->unitsContext.yCoords
    );

    Game::Gameplay::Enemies::generateEnemyBullets(
        dt,
        this->weaponContext.enemyWeaponCooldownTimeLeft,
        this->enemyBulletsContext.xCoords,
        this->enemyBulletsContext.yCoords,
        this->weaponContext.rand,
        Dod::BufferUtils::createImFromBuffer(this->unitsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->unitsContext.yCoords)
    );

    Game::Core::Bullets::updateMovement(
        this->enemyBulletsContext.yCoords,
        120.f,
        dt
    );

    Game::Core::Collision::pointsPlaneIntersection(
        this->enemyBulletsContext.toRemove,
        Dod::BufferUtils::createImFromBuffer(this->enemyBulletsContext.yCoords),
        875.f, 
        -1.f
    );

    Game::Gameplay::Player::testWithBullets(
        this->enemyBulletsContext.toRemove,
        Dod::BufferUtils::createImFromBuffer(this->enemyBulletsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->enemyBulletsContext.yCoords),
        this->playerLifetimeContext.lifes,
        this->playerPositionContext.xCoord,
        this->playerPositionContext.yCoord,
        50.f,
        25.f
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
        Dod::BufferUtils::createImFromBuffer(this->unitsContext.xCoords),
        Dod::BufferUtils::createImFromBuffer(this->unitsContext.yCoords)
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

}
