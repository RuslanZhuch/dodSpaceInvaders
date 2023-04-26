
#include "game.h"
#include <game/renderer/instantRenderer.h>
#include <array>
#include <span>
#include <cassert>
#include <chrono>
#include <iostream>
#include <format>
#include <random>

#include <dod/MemTypes.h>
#include <dod/MemPool.h>
#include <dod/MemUtils.h>
#include <dod/BufferUtils.h>
#include <dod/Algorithms.h>
#include <dod/ConditionTable.h>

#include <game/Scene.h>
#include <game/Inputs.h>
#include <game/Enemies.h>

constexpr auto windowWidth{ 800 };
constexpr auto windowHeight{ 900 };

constexpr auto filedSizeCoeff{ 0.95f };

struct SceneContext
{

    struct EnemiesParamets
    {
        static constexpr auto numOfEnemiesPerRow{ 10 };
        static constexpr auto numOfEnemiesCols{ 4 };
        static constexpr auto enemiesXStride{ 50.f };
        static constexpr auto enemiesYStride{ 50.f };
    };

    struct Player
    {
        float prevMoveComponent{};
    };

    static constexpr auto obstaclesStride{ 30.f };
    static constexpr auto obstaclesPerRow{ 5 };
    static constexpr auto obstaclesPerCol{ 2 };
    static constexpr auto obstaclesClusters{ 3 };
    static constexpr auto obstaclesClustersTotalAreaWidth{ 700 };

    static constexpr auto totalBytesForScene{ 1024 * 1024 * 10 };

    SceneContext()
        :
        memory(totalBytesForScene)
    {

        constexpr auto totalObstacles{ obstaclesPerRow * obstaclesPerCol * obstaclesClusters + 1 };
        constexpr auto bytesToAquire{ totalObstacles * 4 };

        int32_t header{ 0 };

        Dod::BufferUtils::initFromMemory(this->obstaclesX, Dod::MemUtils::stackAquire(this->memory, bytesToAquire, header));
        Dod::BufferUtils::initFromMemory(this->obstaclesY, Dod::MemUtils::stackAquire(this->memory, bytesToAquire, header));
        Dod::BufferUtils::initFromMemory(this->obstaclesLifes, Dod::MemUtils::stackAquire(this->memory, bytesToAquire, header));
        Dod::BufferUtils::initFromMemory(this->obstaclesToHit, Dod::MemUtils::stackAquire(this->memory, bytesToAquire, header));

        Dod::BufferUtils::initFromMemory(this->playerBulletIdsToRemove, Dod::MemUtils::stackAquire(this->memory, 1024, header));
        Dod::BufferUtils::initFromMemory(this->enemyBulletIdsToRemove, Dod::MemUtils::stackAquire(this->memory, 1024, header));

        constexpr auto totalEnemies{ EnemiesParamets::numOfEnemiesCols * EnemiesParamets::numOfEnemiesPerRow + 1 };
        constexpr auto enemiesBytesToAquire{ totalEnemies * 4 };
        Dod::BufferUtils::initFromMemory(this->enemiesXCoords, Dod::MemUtils::stackAquire(this->memory, enemiesBytesToAquire, header));
        Dod::BufferUtils::initFromMemory(this->enemiesYCoords, Dod::MemUtils::stackAquire(this->memory, enemiesBytesToAquire, header));
        Dod::BufferUtils::initFromMemory(this->enemiesToRemove, Dod::MemUtils::stackAquire(this->memory, enemiesBytesToAquire, header));

        constexpr auto maxinumBullets{ 512 };
        constexpr auto bulletsBytesToAquire{ maxinumBullets * 4 };
        Dod::BufferUtils::initFromMemory(this->playerBulletXCoords, Dod::MemUtils::stackAquire(this->memory, bulletsBytesToAquire, header));
        Dod::BufferUtils::initFromMemory(this->playerBulletYCoords, Dod::MemUtils::stackAquire(this->memory, bulletsBytesToAquire, header));
        Dod::BufferUtils::initFromMemory(this->enemyBulletXCoords, Dod::MemUtils::stackAquire(this->memory, bulletsBytesToAquire, header));
        Dod::BufferUtils::initFromMemory(this->enemyBulletYCoords, Dod::MemUtils::stackAquire(this->memory, bulletsBytesToAquire, header));

    }

    Dod::DBBuffer<float> playerBulletXCoords;
    Dod::DBBuffer<float> playerBulletYCoords;

    Dod::DBBuffer<float> enemyBulletXCoords;
    Dod::DBBuffer<float> enemyBulletYCoords;

    Dod::DBBuffer<float> enemiesXCoords;
    Dod::DBBuffer<float> enemiesYCoords;
    Dod::DBBuffer<int32_t> enemiesToRemove;
    
    Dod::DBBuffer<float> obstaclesX;
    Dod::DBBuffer<float> obstaclesY;
    Dod::DBBuffer<int32_t> obstaclesLifes;
    Dod::DBBuffer<int32_t> obstaclesToHit;

    Dod::DBBuffer<int32_t> playerBulletIdsToRemove;
    Dod::DBBuffer<int32_t> enemyBulletIdsToRemove;

    Dod::MemPool memory;

    Player player;

};

void drawObstacles(Game::GameRenderer& renderer, const SceneContext& scene)
{
    for (int32_t elId{0}; elId < scene.obstaclesX.numOfFilledEls; ++elId)
    {
        constexpr auto length{ SceneContext::obstaclesStride };
        const auto x{ Dod::BufferUtils::get(scene.obstaclesX, elId) };
        const auto y{ Dod::BufferUtils::get(scene.obstaclesY, elId) };
        Game::Scene::drawObstacle(renderer, x, y);
    }
}

void drawBullets(Game::GameRenderer& renderer, Dod::DBBuffer<float> xPositions, Dod::DBBuffer<float> yPositions)
{
    for (int32_t elId{ 0 }; elId < xPositions.numOfFilledEls; ++elId)
        Game::Scene::drawBullet(renderer, Dod::BufferUtils::get(xPositions, elId), Dod::BufferUtils::get(yPositions, elId));
}

void drawEnemies(Game::GameRenderer& renderer, const SceneContext& scene)
{
    for (int32_t elId{0}; elId < scene.enemiesXCoords.numOfFilledEls; ++elId)
        Game::Scene::drawEnemy(renderer, Dod::BufferUtils::get(scene.enemiesXCoords, elId), Dod::BufferUtils::get(scene.enemiesYCoords, elId));
}

constexpr uint32_t moveLeftControlBit{ uint32_t(1) << 0 };
constexpr uint32_t moveRightControlBit{ uint32_t(1) << 1 };
constexpr uint32_t fireControlBit{ uint32_t(1) << 2 };

uint32_t gatherInputs()
{

    uint32_t inputBits{};

    inputBits |= (moveLeftControlBit)*sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    inputBits |= (moveRightControlBit)*sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    inputBits |= (fireControlBit)*sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

    return inputBits;

}

static std::random_device gRandomDevice;
static std::mt19937 gRandomGen(gRandomDevice());

static float gEnemyBulletDelayTimeLeft;

static float gEnemiesBatchTargetX{};
static float gEnemiesBatchCoordX{};
static float gEnemiesBatchCoordY{};
static float gEnemiesBatchMoveTimeleft{};
static float gEnemiesDirection = 1.f;

auto enemiesBatchUpdate(float dt, float currentDirection, float batchCoordX, float batchCoordY)
{

    const auto currentBatchMoveTimeLeft{ Game::Enemies::updateStrobeCountdown(dt, gEnemiesBatchMoveTimeleft, 1.f) };
    const auto bNeedMove{ Game::Enemies::updateStrobe(gEnemiesBatchMoveTimeleft, currentBatchMoveTimeLeft) };
    gEnemiesBatchMoveTimeleft = currentBatchMoveTimeLeft;

    const auto bNeedChangeBatchDirection{ Game::Enemies::computeNeedChangeDirection(currentDirection, batchCoordX, 75.f, 275.f) && bNeedMove };

    const auto newBatchDirection{ Game::Enemies::computeNewDirection(currentDirection, bNeedChangeBatchDirection) };
    const auto newBatchXCoord{ Game::Enemies::computeNewXPosition(newBatchDirection, batchCoordX, 25.f, bNeedMove) };

    const auto newBatchYCoord{ Game::Enemies::computeNewYPosition(batchCoordY, 25.f, bNeedChangeBatchDirection) };

    struct Output 
    {
        float batchXCoord{};
        float batchYCoord{};
        float batchDirection{};
    };
    return Output(newBatchXCoord, newBatchYCoord, newBatchDirection);

}

void enemiesLifetimeUpdate(SceneContext& scene)
{

    const auto enemiesToRemove{ Dod::BufferUtils::createImFromBuffer(scene.enemiesToRemove) };

    Dod::BufferUtils::remove(scene.enemiesXCoords, enemiesToRemove);
    Dod::BufferUtils::remove(scene.enemiesYCoords, enemiesToRemove);

    scene.enemiesToRemove.numOfFilledEls = 0;

}

auto enemiesUpdate(float dt, float currentDirection, float batchTargetX, float batchCoordX, float batchCoordY, SceneContext& scene)
{

    const auto [newBatchTargetX, newBatchCoordY, newCurrentDirection] {enemiesBatchUpdate(dt, currentDirection, batchTargetX, batchCoordY)};

    const auto moveXDelata{ newBatchTargetX - batchCoordX };
    const auto moveYDelata{ newBatchCoordY - batchCoordY };

    const auto moveX{ moveXDelata * 10.f * dt };
    const auto newBatchCoordX{ batchCoordX += moveX };
    for (int32_t elId{0}; elId < scene.enemiesXCoords.numOfFilledEls; ++elId)
        Dod::BufferUtils::get(scene.enemiesXCoords, elId) += moveX;

    for (int32_t elId{ 0 }; elId < scene.enemiesYCoords.numOfFilledEls; ++elId)
        Dod::BufferUtils::get(scene.enemiesYCoords, elId) += moveYDelata;

    struct Output
    {
        float batchTargetX{};
        float batchXCoord{};
        float batchYCoord{};
        float batchDirection{};
    };

    return Output(newBatchTargetX, newBatchCoordX, newBatchCoordY, newCurrentDirection);

}

auto generateEnemyBulletRule(float dt, float& delayTimeleft)
{

    std::uniform_int_distribution<> distrib(0, 1000);
    const auto randValue{ distrib(gRandomGen) };
    const auto bNeedCreateBullet{ randValue > 250 };

    delayTimeleft -= dt;
    const auto bDelayCompleted{ delayTimeleft <= 0.f };
    delayTimeleft = delayTimeleft * !bDelayCompleted + 0.5f * bDelayCompleted;

    return bNeedCreateBullet && bDelayCompleted;

}

auto generateEnemyBulletSourceIdRule(int32_t numOfSources)
{

    std::uniform_int_distribution<> distrib(1 * (numOfSources > 0), numOfSources);
    const auto sourceId{ distrib(gRandomGen) };

    return sourceId;

}

void generateEnemyBullets(
    SceneContext& scene,
    bool bNeedCreateBullet
)
{

    bNeedCreateBullet &= scene.enemiesXCoords.numOfFilledEls > 0;

    const auto bulletXId{ generateEnemyBulletSourceIdRule(scene.enemiesXCoords.numOfFilledEls) };
    const auto bulletYId{ generateEnemyBulletSourceIdRule(scene.enemiesXCoords.numOfFilledEls) };

    Dod::BufferUtils::populate(scene.enemyBulletXCoords, scene.enemiesXCoords.dataBegin[bulletXId], bNeedCreateBullet);
    Dod::BufferUtils::populate(scene.enemyBulletYCoords, scene.enemiesYCoords.dataBegin[bulletYId], bNeedCreateBullet);

}

void getAxisCollisionsList(Dod::DBBuffer<uint64_t>& collisions, const Dod::ImBuffer<float>& lefts, const Dod::ImBuffer<float>& rights)
{

    constexpr float maxDistForCollision{ 15.f };
    constexpr float maxDistForCollisionSqr{ maxDistForCollision * maxDistForCollision };

    for (int32_t leftIdx{ 0 }; leftIdx < lefts.numOfFilledEls; ++leftIdx)
    {
        for (int32_t rightIdx{ 0 }; rightIdx < rights.numOfFilledEls; ++rightIdx)
        {
            const auto distanceSqr{ std::powf(Dod::BufferUtils::get(rights, rightIdx) - Dod::BufferUtils::get(lefts, leftIdx), 2)};
            const auto bInRange{ distanceSqr <= maxDistForCollisionSqr };
            Dod::BufferUtils::populate(collisions, (static_cast<uint64_t>(leftIdx) << 32) | rightIdx, bInRange);
        }
    }

}

void bulletsCollisionUpdate(SceneContext& scene)
{

    std::array<uint64_t, 128> xAxisCollidedMem;
    Dod::DBBuffer<uint64_t> xAxisCollided;
    Dod::BufferUtils::initFromArray(xAxisCollided, xAxisCollidedMem);

    std::array<uint64_t, 128> yAxisCollidedMem;
    Dod::DBBuffer<uint64_t> yAxisCollided;
    Dod::BufferUtils::initFromArray(yAxisCollided, yAxisCollidedMem);

    getAxisCollisionsList(xAxisCollided, Dod::BufferUtils::createImFromBuffer(scene.playerBulletXCoords), Dod::BufferUtils::createImFromBuffer(scene.enemiesXCoords));
    getAxisCollisionsList(yAxisCollided, Dod::BufferUtils::createImFromBuffer(scene.playerBulletYCoords), Dod::BufferUtils::createImFromBuffer(scene.enemiesYCoords));

    std::array<uint64_t, 512> collidedMem;
    Dod::DBBuffer<uint64_t> collided;
    Dod::BufferUtils::initFromArray(collided, collidedMem);

    Dod::Algorithms::getIntersections(collided, xAxisCollided, yAxisCollided);

    for (int32_t elId{}; elId < collided.numOfFilledEls; ++elId)
        Dod::BufferUtils::populate(scene.enemiesToRemove, static_cast<int32_t>(Dod::BufferUtils::get(collided, elId)), true);
 
    for (int32_t elId{}; elId < collided.numOfFilledEls; ++elId)
        Dod::BufferUtils::populate(scene.playerBulletIdsToRemove, static_cast<int32_t>(Dod::BufferUtils::get(collided, elId) >> 32), true);

}

void bulletsMovementUpdate(Dod::DBBuffer<float> bulletsY, float dt, float bulletVelocity)
{
    for (int32_t idx{ 0 }; idx < bulletsY.numOfFilledEls; ++idx) {
        Dod::BufferUtils::get(bulletsY, idx) += bulletVelocity * dt;
    }
}

void bulletsLifetimeUpdate(Dod::DBBuffer<float>& bulletsX, Dod::DBBuffer<float>& bulletsY, Dod::DBBuffer<int32_t>& bulletIdsToRemove)
{

    std::sort(bulletIdsToRemove.dataBegin + 1, bulletIdsToRemove.dataBegin + 1 + bulletIdsToRemove.numOfFilledEls, std::greater());
    Dod::Algorithms::leftUniques(bulletIdsToRemove);

    const auto bulletsToRemoveIm{ Dod::BufferUtils::createImFromBuffer(bulletIdsToRemove) };

    Dod::BufferUtils::remove(bulletsX, bulletsToRemoveIm);
    Dod::BufferUtils::remove(bulletsY, bulletsToRemoveIm);

    bulletIdsToRemove.numOfFilledEls = 0;

}

void bulletsPlaneCollision(const Dod::DBBuffer<float>& bulletsY, Dod::DBBuffer<int32_t>& bulletsToRemove, float planeY, float dir)
{

    size_t removeListSize{ 0 };
    std::array<int32_t, 255> removeList;
    for (int32_t bulletId{ 0 }; bulletId < bulletsY.numOfFilledEls; ++bulletId)
    {
        const auto bIsColliding{ Dod::BufferUtils::get(bulletsY, bulletId) * dir < planeY * dir };
        removeListSize += size_t(1) * bIsColliding;
        removeList[removeListSize * bIsColliding] = bulletId;
    }

    for (size_t bulletId{ 1 }; bulletId < removeListSize + 1; ++bulletId)
        Dod::BufferUtils::populate(bulletsToRemove, removeList[bulletId], true);

}

void pointsAreasIntersection(
    Dod::DBBuffer<uint64_t>& intersections,
    const Dod::ImBuffer<float>& pointsCenter,
    const Dod::ImBuffer<float>& areasCenter,
    const float areasWidth
)
{

    for (int32_t pointId{ 0 }; pointId < pointsCenter.numOfFilledEls; ++pointId)
    {
        const auto pointCenter{ Dod::BufferUtils::get(pointsCenter, pointId) };
        for (int32_t areaId{ 0 }; areaId < areasCenter.numOfFilledEls; ++areaId)
        {
            const auto areaCenter{ Dod::BufferUtils::get(areasCenter, areaId) };
            const auto areaWidth{ areasWidth };
            const auto areaHalfWidth{ areaWidth * 0.5f };
            const auto areaLeft{ areaCenter - areaHalfWidth };
            const auto areaRight{ areaCenter + areaHalfWidth };

            const auto bInArea{ (pointCenter >= areaLeft) && (pointCenter <= areaRight) };

            Dod::BufferUtils::populate(intersections, (static_cast<uint64_t>(pointId) << 32) | areaId, bInArea);
        }
    }

}

[[nodiscard]] auto pointsSquaresIntersection(
    Dod::DBBuffer<uint64_t>& intersections,
    const Dod::ImBuffer<float>& pointsX,
    const Dod::ImBuffer<float>& pointsY,
    const Dod::ImBuffer<float>& areasX,
    const Dod::ImBuffer<float>& areasY,
    const float areasWidth,
    const float areasHeight
)
{

    std::array<uint64_t, 128> intersectionsXMem;
    Dod::DBBuffer<uint64_t> intersectionsX;
    Dod::BufferUtils::initFromArray(intersectionsX, intersectionsXMem);

    std::array<uint64_t, 128> intersectionsYMem;
    Dod::DBBuffer<uint64_t> intersectionsY;
    Dod::BufferUtils::initFromArray(intersectionsY, intersectionsYMem);

    pointsAreasIntersection(intersectionsX, pointsX, areasX, areasWidth);
    pointsAreasIntersection(intersectionsY, pointsY, areasY, areasHeight);

    Dod::Algorithms::getIntersections(intersections, intersectionsX, intersectionsY);

}

void testBulletsWithObstacles(
    const Dod::DBBuffer<float>& bulletsX,
    const Dod::DBBuffer<float>& bulletsY,
    Dod::DBBuffer<int32_t>& bulletsToDelete,
    SceneContext& scene
)
{

    std::array<uint64_t, 128> intersectionsMem;
    Dod::DBBuffer<uint64_t> intersections;
    Dod::BufferUtils::initFromArray(intersections, intersectionsMem);

    pointsSquaresIntersection(
        intersections,
        Dod::BufferUtils::createImFromBuffer(bulletsX),
        Dod::BufferUtils::createImFromBuffer(bulletsY),
        Dod::BufferUtils::createImFromBuffer(scene.obstaclesX),
        Dod::BufferUtils::createImFromBuffer(scene.obstaclesY),
        SceneContext::obstaclesStride,
        SceneContext::obstaclesStride
    );


    for (int32_t elId{}; elId < intersections.numOfFilledEls; ++elId)
        Dod::BufferUtils::populate(scene.obstaclesToHit, static_cast<int32_t>(Dod::BufferUtils::get(intersections, elId)), true);

    for (int32_t elId{}; elId < intersections.numOfFilledEls; ++elId)
        Dod::BufferUtils::populate(bulletsToDelete, static_cast<int32_t>(Dod::BufferUtils::get(intersections, elId) >> 32), true);

}

void pointsAreaIntersection(
    Dod::DBBuffer<int32_t>& intersections,
    const Dod::ImBuffer<float>& pointsCenter,
    const float areaCenter,
    const float areaWidth
)
{

    for (int32_t pointId{ 0 }; pointId < pointsCenter.numOfFilledEls; ++pointId)
    {
        const auto pointCenter{ Dod::BufferUtils::get(pointsCenter, pointId) };

        const auto areaHalfWidth{ areaWidth * 0.5f };
        const auto areaLeft{ areaCenter - areaHalfWidth };
        const auto areaRight{ areaCenter + areaHalfWidth };

        const auto bInArea{ (pointCenter >= areaLeft) && (pointCenter <= areaRight) };

        Dod::BufferUtils::populate(intersections, pointId, bInArea);
    }

}

void pointsSquareIntersection(
    Dod::DBBuffer<int32_t>& intersections,
    const Dod::ImBuffer<float>& pointsX,
    const Dod::ImBuffer<float>& pointsY,
    const float areaX,
    const float areaY,
    const float areaWidth,
    const float areaHeight
)
{

    std::array<int32_t, 128> intersectionsXMem;
    Dod::DBBuffer<int32_t> intersectionsX;
    Dod::BufferUtils::initFromArray(intersectionsX, intersectionsXMem);

    std::array<int32_t, 128> intersectionsYMem;
    Dod::DBBuffer<int32_t> intersectionsY;
    Dod::BufferUtils::initFromArray(intersectionsY, intersectionsYMem);

    pointsAreaIntersection(intersectionsX, pointsX, areaX, areaWidth);
    pointsAreaIntersection(intersectionsY, pointsY, areaY, areaHeight);

    Dod::Algorithms::getIntersections(intersections, intersectionsX, intersectionsY);

}

[[nodiscard]] auto testBulletsWithPlayer(
    const Dod::ImBuffer<float>& bulletsX,
    const Dod::ImBuffer<float>& bulletsY,
    Dod::DBBuffer<int32_t>& bulletsToDelete,
    const float playerX,
    const float playerY,
    const int32_t playerLifes
)
{

    const auto prevNumBulletsToDelete{ bulletsToDelete.numOfFilledEls };

    pointsSquareIntersection(
        bulletsToDelete,
        bulletsX,
        bulletsY,
        playerX,
        playerY,
        60,
        30
    );

    const auto currNumBulletsToDelete{ bulletsToDelete.numOfFilledEls };

    return playerLifes - int32_t(1) * (currNumBulletsToDelete - prevNumBulletsToDelete > 0);

}

static sf::Vector2f gPlayerPosition;
static int32_t gPlayerLives{ 1 };
static uint32_t gPrevInputBits;

void playerUpdate(float dt, SceneContext& scene)
{

    const auto inputBits{ gatherInputs() };

    const auto fireComponent{ Game::Inputs::computeFireComponent(inputBits, gPrevInputBits) };
    const auto moveComponent{ Game::Inputs::computeMoveComponent(inputBits, gPrevInputBits, scene.player.prevMoveComponent) };

    gPrevInputBits = inputBits;

    scene.player.prevMoveComponent = moveComponent;

    gPlayerPosition.x += moveComponent * 200.f * dt;

    const auto bNeedCreateBullet{ (fireComponent > 0) && (gPlayerLives > 0)};

    Dod::BufferUtils::populate(scene.playerBulletXCoords, gPlayerPosition.x, bNeedCreateBullet);
    Dod::BufferUtils::populate(scene.playerBulletYCoords, gPlayerPosition.y, bNeedCreateBullet);

}

void obstaclesLifetimeUpdate(SceneContext& scene)
{

    std::array<int32_t, 256> obstaclesToRemoveMemory;
    Dod::DBBuffer<int32_t> obstaclesToRemove;
    Dod::BufferUtils::initFromArray(obstaclesToRemove, obstaclesToRemoveMemory);

    for (int32_t elId{ 0 }; elId < scene.obstaclesToHit.numOfFilledEls; ++elId)
    {
        const auto obstacleToHitId{ Dod::BufferUtils::get(scene.obstaclesToHit, elId) };
        const auto lifesLeft{ --Dod::BufferUtils::get(scene.obstaclesLifes, obstacleToHitId)};
        const auto bNeedToRemove{ lifesLeft == 0 };

        Dod::BufferUtils::populate(obstaclesToRemove, obstacleToHitId, bNeedToRemove);
    }

    const auto obstaclesToRemoveIm{ Dod::BufferUtils::createImFromBuffer(obstaclesToRemove) };

    Dod::BufferUtils::remove(scene.obstaclesX, obstaclesToRemoveIm);
    Dod::BufferUtils::remove(scene.obstaclesY, obstaclesToRemoveIm);
    Dod::BufferUtils::remove(scene.obstaclesLifes, obstaclesToRemoveIm);

    scene.obstaclesToHit.numOfFilledEls = 0;

}

void generateObstacles(SceneContext& scene)
{

    const auto totalObstaclesPerCluster{ SceneContext::obstaclesPerRow * SceneContext::obstaclesPerCol };

    const auto widthPerCluster{ SceneContext::obstaclesPerRow * SceneContext::obstaclesStride };
    const auto totalObstaclesWidth{ widthPerCluster * SceneContext::obstaclesClusters };
    const auto needObstaclesWidth{ SceneContext::obstaclesClustersTotalAreaWidth };
    const auto totalGapWidth{ needObstaclesWidth - totalObstaclesWidth };
    const auto numOfClusterGaps{ SceneContext::obstaclesClusters - 1 };
    const auto gapWidth{ totalGapWidth / numOfClusterGaps };
    const auto distanceBetweenClusters{ gapWidth + widthPerCluster };

    const auto obstaclesClusterY{ 650.f };

    const auto initialClusterX{ windowWidth * 0.5f - needObstaclesWidth * 0.5f };

    for (int32_t clusterId{ 0 }; clusterId < SceneContext::obstaclesClusters; ++clusterId)
    {
        const auto obstaclesClusterX{ initialClusterX + distanceBetweenClusters * static_cast<float>(clusterId) };
        for (int32_t obstacleId{ 0 }; obstacleId < totalObstaclesPerCluster; ++obstacleId)
        {
            const auto colId{ obstacleId % SceneContext::obstaclesPerRow };
            const auto rowId{ obstacleId / SceneContext::obstaclesPerRow };

            const auto x{ obstaclesClusterX + static_cast<float>(colId) * SceneContext::obstaclesStride };
            const auto y{ obstaclesClusterY + static_cast<float>(rowId) * SceneContext::obstaclesStride };

            Dod::BufferUtils::populate(scene.obstaclesX, x, true);
            Dod::BufferUtils::populate(scene.obstaclesY, y, true);
            Dod::BufferUtils::populate(scene.obstaclesLifes, 1, true);
        }
    }
     
}

void msgLoop(Game::GameRenderer& renderer, float dt, SceneContext& scene)
{

    sf::Event event;
    while (renderer.getWindow().pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            renderer.getWindow().close();
    }

    renderer.getWindow().clear();

    playerUpdate(dt, scene);
    bulletsMovementUpdate(scene.playerBulletYCoords, dt, -220.f);
    bulletsCollisionUpdate(scene);
    bulletsPlaneCollision(scene.playerBulletYCoords, scene.playerBulletIdsToRemove, 100.f, 1.f);
    testBulletsWithObstacles(
        scene.playerBulletXCoords,
        scene.playerBulletYCoords,
        scene.playerBulletIdsToRemove,
        scene
    );

    bulletsLifetimeUpdate(scene.playerBulletXCoords, scene.playerBulletYCoords, scene.playerBulletIdsToRemove);

    enemiesLifetimeUpdate(scene);
    const auto enemiesResult {enemiesUpdate(
        dt, 
        gEnemiesDirection, 
        gEnemiesBatchTargetX, 
        gEnemiesBatchCoordX, 
        gEnemiesBatchCoordY, 
        scene
    ) };
    gEnemiesDirection = enemiesResult.batchDirection;
    gEnemiesBatchTargetX = enemiesResult.batchTargetX;
    gEnemiesBatchCoordX = enemiesResult.batchXCoord;
    gEnemiesBatchCoordY = enemiesResult.batchYCoord;

    generateEnemyBullets(scene, generateEnemyBulletRule(dt, gEnemyBulletDelayTimeLeft));
    bulletsMovementUpdate(scene.enemyBulletYCoords, dt, 120.f);

    bulletsPlaneCollision(scene.enemyBulletYCoords, scene.enemyBulletIdsToRemove, 875.f, -1.f);

    gPlayerLives = testBulletsWithPlayer(
        Dod::BufferUtils::createImFromBuffer(scene.enemyBulletXCoords), 
        Dod::BufferUtils::createImFromBuffer(scene.enemyBulletYCoords), 
        scene.enemyBulletIdsToRemove, 
        gPlayerPosition.x, 
        gPlayerPosition.y, 
        gPlayerLives);

    testBulletsWithObstacles(
        scene.enemyBulletXCoords,
        scene.enemyBulletYCoords,
        scene.enemyBulletIdsToRemove,
        scene
    );
    bulletsLifetimeUpdate(scene.enemyBulletXCoords, scene.enemyBulletYCoords, scene.enemyBulletIdsToRemove);

    obstaclesLifetimeUpdate(scene);

    drawEnemies(renderer, scene);

    drawObstacles(renderer, scene);

    Game::Scene::drawField(renderer);
    Game::Scene::drawPlayer(renderer, gPlayerPosition, gPlayerLives);

    drawBullets(renderer, scene.playerBulletXCoords, scene.playerBulletYCoords);
    drawBullets(renderer, scene.enemyBulletXCoords, scene.enemyBulletYCoords);

    renderer.getWindow().display();

}

void Game::run()
{

    Game::GameRenderer renderer(windowWidth, windowHeight, "dod Space Invaders");
    SceneContext scene;

    gPlayerPosition = sf::Vector2f(400.f, 850.f);

    gEnemiesBatchCoordX = 100.f;
    gEnemiesBatchCoordY = 200.f;

    Game::Enemies::generateX(
        scene.enemiesXCoords,
        SceneContext::EnemiesParamets::numOfEnemiesPerRow, SceneContext::EnemiesParamets::numOfEnemiesCols,
        gEnemiesBatchCoordX, SceneContext::EnemiesParamets::enemiesXStride
    );
    Game::Enemies::generateY(
        scene.enemiesYCoords,
        SceneContext::EnemiesParamets::numOfEnemiesPerRow, SceneContext::EnemiesParamets::numOfEnemiesCols,
        gEnemiesBatchCoordY, SceneContext::EnemiesParamets::enemiesYStride
    );

    generateObstacles(scene);

    float deltaTime{ 0.f };

    while (renderer.getWindow().isOpen())
    {
        const auto start{ std::chrono::high_resolution_clock::now() };
        msgLoop(renderer, deltaTime, scene);
        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;
    }

}
