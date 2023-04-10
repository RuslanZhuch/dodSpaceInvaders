
#include "game.h"
#include "../renderer/instantRenderer.h"
#include <SFML/Graphics.hpp>
#include <span>
#include <array>
#include <cassert>
#include <chrono>
#include <iostream>
#include <bitset>
#include <ranges>
#include <format>
#include <random>
#include <range/v3/view.hpp>
#include <range/v3/span.hpp>
#include <range/v3/view/zip.hpp>

#include <dod/MemTypes.h>
#include <dod/MemPool.h>
#include <dod/MemUtils.h>
#include <dod/BufferUtils.h>
#include <dod/Algorithms.h>

constexpr auto windowWidth{ 800 };
constexpr auto windowHeight{ 900 };

constexpr auto numOfCells{ 20 };
constexpr auto filedSizeCoeff{ 0.95f };
constexpr auto numOfEnemiesPerRow{ 10 };
constexpr auto numOfEnemiesCols{ 4 };
constexpr auto enemiesXStride{ 50.f };
constexpr auto enemiesYStride{ 50.f };


struct Scene
{

    static constexpr auto obstaclesStride{ 30.f };
    static constexpr auto obstaclesPerRow{ 5 };
    static constexpr auto obstaclesPerCol{ 2 };
    static constexpr auto obstaclesClusters{ 3 };
    static constexpr auto obstaclesClustersTotalAreaWidth{ 700 };

    static constexpr auto totalBytesForScene{ 1024 * 1024 * 10 };

    Scene()
        :
        memory(totalBytesForScene)
    {

        constexpr auto totalObstacles{ obstaclesPerRow * obstaclesPerCol * obstaclesClusters + 1 };
        constexpr auto bytesToAquire{ totalObstacles * 4 };

        size_t header{ 0 };

        Dod::BufferUtils::initFromMemory(this->obstaclesX, Dod::MemUtils::stackAquire(this->memory, bytesToAquire, header), 0, bytesToAquire);
        Dod::BufferUtils::initFromMemory(this->obstaclesY, Dod::MemUtils::stackAquire(this->memory, bytesToAquire, header), 0, bytesToAquire);
        Dod::BufferUtils::initFromMemory(this->obstaclesLifes, Dod::MemUtils::stackAquire(this->memory, bytesToAquire, header), 0, bytesToAquire);
        Dod::BufferUtils::initFromMemory(this->obstaclesToHit, Dod::MemUtils::stackAquire(this->memory, bytesToAquire, header), 0, bytesToAquire);

        Dod::BufferUtils::initFromMemory(this->playerBulletIdsToRemove, Dod::MemUtils::stackAquire(this->memory, 1024, header), 0, 1024);
        Dod::BufferUtils::initFromMemory(this->enemyBulletIdsToRemove, Dod::MemUtils::stackAquire(this->memory, 1024, header), 0, 1024);

    }

    Dod::DBBuffer<float> obstaclesX;
    Dod::DBBuffer<float> obstaclesY;
    Dod::DBBuffer<int32_t> obstaclesLifes;
    Dod::DBBuffer<int32_t> obstaclesToHit;

    Dod::DBBuffer<int32_t> playerBulletIdsToRemove;
    Dod::DBBuffer<int32_t> enemyBulletIdsToRemove;

    Dod::MemPool memory;

};

const auto getFieldSize(sf::RenderWindow& window)
{

    const auto windowSize{ window.getSize() };
    return sf::Vector2f(windowSize.x * filedSizeCoeff, windowSize.y * filedSizeCoeff);

}
const auto getCornerPoint(sf::RenderWindow& window)
{

    const auto windowSize{ window.getSize() };
    return sf::Vector2f(windowSize.x, windowSize.x) * (1.f - filedSizeCoeff) * 0.5f;

}

void drawField(sf::RenderWindow& window)
{

    const auto windowSize{ window.getSize() };
    const auto centerPoint{ sf::Vector2f(windowSize) * 0.5f };
    const auto fieldSize{ sf::Vector2f(windowSize) * filedSizeCoeff };

    const auto color{ sf::Color(200, 100, 50, 200) };
    Renderer::Instant::drawRectangle(window, centerPoint, fieldSize, color, 2.f);

}

void drawPlayer(sf::RenderWindow& window, const sf::Vector2f& position, const int32_t playerLifes)
{

    const auto radius{ 25.f * (playerLifes > 0) };
    const auto leftPoint{ position - sf::Vector2f(-radius, -radius * 0.5f) };
    const auto rightPoint{ position - sf::Vector2f(radius, -radius * 0.5f) };
    const auto topPoint{ position - sf::Vector2f(0.f, radius * 0.5f) };

    const auto playerColor{ sf::Color(150.f, 200.f, 90.f) };
    Renderer::Instant::drawLine(window, leftPoint, rightPoint, playerColor);
    Renderer::Instant::drawLine(window, leftPoint, topPoint, playerColor);
    Renderer::Instant::drawLine(window, rightPoint, topPoint, playerColor);

}

void drawObstacles(sf::RenderWindow& window, Scene& scene)
{
    for (int32_t elId{0}; elId < scene.obstaclesX.numOfFilledEls; ++elId)
    {
        constexpr auto length{ Scene::obstaclesStride };
        const auto x{ Dod::BufferUtils::get(scene.obstaclesX, elId) };
        const auto y{ Dod::BufferUtils::get(scene.obstaclesY, elId) };
        Renderer::Instant::drawRectangle(window, {x, y}, sf::Vector2f(length, length), sf::Color::Green, 1.f, sf::Color::White);
    }
}

void drawBullet(sf::RenderWindow& window, const sf::Vector2f& position)
{
    constexpr auto length{ 15.f };
    Renderer::Instant::drawRectangle(window, position, sf::Vector2f(6.f, length), sf::Color::Yellow, 1.f, sf::Color::Green);
}

void drawBullets(sf::RenderWindow& window, const std::span<const float> xPositions, const std::span<const float> yPositions)
{
    for (const auto [x, y] : ranges::views::zip(xPositions.subspan(1), yPositions.subspan(1)))
    {
        drawBullet(window, { x, y });
    }
}

void drawEnemy(sf::RenderWindow& window, const sf::Vector2f& position)
{
    constexpr auto sizeX{ 25.f };
    constexpr auto sizeY{ 30.f };
    Renderer::Instant::drawRectangle(window, position, sf::Vector2f(sizeX, sizeY), sf::Color::Blue, 1.f, sf::Color::Red);
}

void drawEnemies(sf::RenderWindow& window, const std::span<const float> xPositions, const std::span<const float> yPositions)
{
    for (const auto [x, y] : ranges::views::zip(xPositions.subspan(1), yPositions.subspan(1)))
        drawEnemy(window, { x, y });
}

struct ControlState
{
    float movementComponent{ 0.f };
    int32_t fireComponent{ 0 };
};

constexpr int32_t moveLeftControlBit{ int32_t(1) << 0 };
constexpr int32_t moveRightControlBit{ int32_t(1) << 1 };
constexpr int32_t fireControlBit{ int32_t(1) << 2 };

uint32_t gatherInputs()
{

    uint32_t inputBits{};

    inputBits |= (moveLeftControlBit)*sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    inputBits |= (moveRightControlBit)*sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    inputBits |= (fireControlBit)*sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

    return inputBits;

}

static ControlState gCurrentControlState;


ControlState inputsUpdate(ControlState currentControlState, const uint32_t newInputBits, const uint32_t prevInputBits)
{

    struct PlayerControlMoveRule
    {
        struct Conditions
        {
            uint32_t inputKey{};
            uint32_t prevInputKey{};
            uint32_t mask{};
        };
        std::array<Conditions, 4> conditions{ {
            Conditions(0, moveLeftControlBit, ~(moveLeftControlBit | moveRightControlBit)),
            Conditions(0, moveRightControlBit, ~(moveLeftControlBit | moveRightControlBit)),
            Conditions(moveLeftControlBit, ~moveLeftControlBit, ~moveLeftControlBit),
            Conditions(moveRightControlBit, ~moveRightControlBit, ~moveRightControlBit),
        } };
        std::array<float, 4> movement{ {
            0.f,
            0.f,
            -1.f,
            1.f,
        } };
    };
    struct PlayerControlFireRule
    {
        struct Conditions
        {
            uint32_t inputKey{};
            uint32_t prevInputKey{};
            uint32_t mask{};
        };
        std::array<Conditions, 3> conditions{ {
            Conditions(0, fireControlBit, ~fireControlBit),
            Conditions(fireControlBit, fireControlBit, ~fireControlBit),
            Conditions(fireControlBit, ~fireControlBit, ~fireControlBit),
            } };
        std::array<int32_t, 3> fire{ {0, 0, 1} };
    };

    ControlState newState{ gCurrentControlState };

    {
        PlayerControlMoveRule controlRule;

        for (size_t idx{ 0 }; idx < controlRule.conditions.size(); ++idx)
        {
            const auto cr1{ !((controlRule.conditions[idx].inputKey ^ newInputBits) & ~controlRule.conditions[idx].mask)};
            const auto cr2{ !((controlRule.conditions[idx].prevInputKey ^ prevInputBits) & ~controlRule.conditions[idx].mask) };
            const auto satisfy{ cr1 && cr2 };

            if (satisfy)
                newState.movementComponent = controlRule.movement[idx] * satisfy + gCurrentControlState.movementComponent * !satisfy;
        }
    }

    {
        PlayerControlFireRule controlRule;

        for (size_t idx{ 0 }; idx < controlRule.conditions.size(); ++idx)
        {
            const auto satisfy{
                !((controlRule.conditions[idx].inputKey ^ newInputBits) & ~controlRule.conditions[idx].mask) &&
                !((controlRule.conditions[idx].prevInputKey ^ prevInputBits) & ~controlRule.conditions[idx].mask)
            };
            if (satisfy)
                newState.fireComponent = controlRule.fire[idx] * satisfy;
        }
    }

    return newState;

}

static std::random_device gRandomDevice;
static std::mt19937 gRandomGen(gRandomDevice());

static std::vector<float> gPlayerBulletXCoords;
static std::vector<float> gPlayerBulletYCoords;
//static std::vector<uint32_t> gPlayerBulletIdsToRemove;

static std::vector<float> gEnemyBulletXCoords;
static std::vector<float> gEnemyBulletYCoords;
//static std::vector<uint32_t> gEnemyBulletIdsToRemove;
static float gEnemyBulletDelayTimeLeft;

static size_t gNumOfRemovedBullets{ 0 };
static std::vector<size_t> gRemovedBulletIds;

static std::vector<float> gEnemiesXCorrds;
static std::vector<float> gEnemiesYCorrds;
static std::vector<uint32_t> gEnemyIdsToRemove;

static float gEnemiesBatchTargetX{};
static float gEnemiesBatchCoordX{};
static float gEnemiesBatchCoordY{};
static float gEnemiesBatchMoveTimeleft{};
static float gEnemiesDirection = 1;

bool updateDirectionRule(float currentDirection, float currentXPosition)
{

    constexpr uint32_t xOnTheLeft{ 1 << 0 };
    constexpr uint32_t xOnTheRight{ 1 << 1 };
    constexpr uint32_t directionLeft{ 1 << 2 };
    constexpr uint32_t directionRight{ 1 << 3 };
    struct DirectionRule
    {
        std::array<uint32_t, 2> conditions{ {
            xOnTheLeft | directionLeft,
            xOnTheRight | directionRight,
        }};
        std::array<bool, 2> outputs{ {true, true} };

        bool outputNeedChangeDirection{ false };
    };

    DirectionRule rule;

    const auto generateInput = [](float currentDirection, float currentXPosition) -> uint32_t {
        uint32_t bits{};
        bits |= (xOnTheLeft) * (currentXPosition < 75.f);
        bits |= (xOnTheRight) * (currentXPosition > 275.f);
        bits |= (directionLeft) * (currentDirection <= -1.f);
        bits |= (directionRight) * (currentDirection >= 1.f);
        return bits;
    };

    const auto inputs{ generateInput(currentDirection, currentXPosition) };

    auto needChangeDirection{ rule.outputNeedChangeDirection };
    for (size_t idx{ 0 }; idx < rule.conditions.size(); ++idx)
    {
        if (rule.conditions[idx] == inputs)
        {
            needChangeDirection = rule.outputs[idx];
        }
    }

    return needChangeDirection;

}

bool enemiesUpdateStrobe(float dt)
{

    gEnemiesBatchMoveTimeleft -= dt;
    const auto bNeedMove{ gEnemiesBatchMoveTimeleft <= 0.f };
    gEnemiesBatchMoveTimeleft += 1.f * bNeedMove;

    return bNeedMove;

}

float enemiesUpdateBatchXCoord(float dt, float currentDirection, float batchCoordX, bool bNeedMove)
{

    constexpr auto batchVelocityX{ 25.f };

    batchCoordX += batchVelocityX * currentDirection * bNeedMove;

    return batchCoordX;

}

float enemiesUpdateBatchDirection(bool bNeedChangeBatchDirection, float currentDirection)
{

    const auto newDirection{ currentDirection + (-currentDirection) * 2.f * bNeedChangeBatchDirection };

    return newDirection;

}

float enemiesUpdateBatchYCoord(bool bBatchDirectionUpdated, float batchYCoord)
{

    constexpr auto stride{ 25.f };
    const auto newBatchYCoord{ batchYCoord + stride * bBatchDirectionUpdated };

    return newBatchYCoord;

}

auto enemiesBatchUpdate(float dt, float currentDirection, float batchCoordX, float batchCoordY)
{

    const auto bNeedMove{ enemiesUpdateStrobe(dt) };

    const auto bNeedChangeBatchDirection{ updateDirectionRule(currentDirection, batchCoordX) * bNeedMove };

    const auto newBatchDirection{ enemiesUpdateBatchDirection(bNeedChangeBatchDirection, currentDirection) };
    const auto newBatchXCoord{ enemiesUpdateBatchXCoord(dt, newBatchDirection, batchCoordX, bNeedMove) };

    const auto newBatchYCoord{ enemiesUpdateBatchYCoord(bNeedChangeBatchDirection, batchCoordY) };

    struct Output 
    {
        float batchXCoord{};
        float batchYCoord{};
        float batchDirection{};
    };
    return Output(newBatchXCoord, newBatchYCoord, newBatchDirection);

}

void enemiesLifetimeUpdate()
{

    const auto applyRemoval = [&](auto& bufferRemoveFrom) -> void {
        size_t targetIdx{ bufferRemoveFrom.size() - 1 };
        for (size_t idx{ 0 }; idx < gEnemyIdsToRemove.size(); ++idx)
        {
            const auto removeId{ gEnemyIdsToRemove[idx] };
            std::swap(bufferRemoveFrom[removeId], bufferRemoveFrom[targetIdx]);
            --targetIdx;
        }
        bufferRemoveFrom.resize(bufferRemoveFrom.size() - gEnemyIdsToRemove.size());
    };

    applyRemoval(gEnemiesXCorrds);
    applyRemoval(gEnemiesYCorrds);

    gEnemyIdsToRemove.clear();

}

auto enemiesUpdate(float dt, float currentDirection, float batchTargetX, float batchCoordX, float batchCoordY, const std::span<float> xCoords, const std::span<float> yCoords)
{

    const auto [newBatchTargetX, newBatchCoordY, newCurrentDirection] {enemiesBatchUpdate(dt, currentDirection, batchTargetX, batchCoordY)};

    const auto moveXDelata{ newBatchTargetX - batchCoordX };
    const auto moveYDelata{ newBatchCoordY - batchCoordY };

    const auto moveX{ moveXDelata * 10.f * dt };
    const auto newBatchCoordX{ batchCoordX += moveX };
    for (auto& xCoord : xCoords)
        xCoord += moveX;

    for (auto& yCoord : yCoords)
        yCoord += moveYDelata;

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

    return bNeedCreateBullet * bDelayCompleted;

}

auto generateEnemyBulletSourceIdRule(size_t numOfSources)
{

    std::uniform_int_distribution<> distrib(1 * (numOfSources > 1), numOfSources - 1);
    const auto sourceId{ distrib(gRandomGen) };

    return sourceId;

}

void generateEnemyBullets(
    const std::span<const float> enemiesX, 
    const std::span<const float> enemiesY,
    std::vector<float>& enemiesBulletsX,
    std::vector<float>& enemiesBulletsY,
    bool bNeedCreateBullet
)
{

    const auto numOfBullets{ enemiesBulletsX.size() - 1 };
    const auto newBulletIdx{ bNeedCreateBullet * (numOfBullets + 1) * (enemiesX.size() > 1)};

    enemiesBulletsX.resize(1 + numOfBullets + 1 * (newBulletIdx > 0));
    enemiesBulletsY.resize(1 + numOfBullets + 1 * (newBulletIdx > 0));

    const auto bulletXId{ generateEnemyBulletSourceIdRule(enemiesX.size())};
    const auto bulletYId{ generateEnemyBulletSourceIdRule(enemiesY.size())};

    enemiesBulletsX[newBulletIdx] = enemiesX[bulletXId];
    enemiesBulletsY[newBulletIdx] = enemiesY[bulletYId];

}

auto getAxisCollisionsList(const std::span<const float> lefts, const std::span<const float> rights)
{

    const auto leftsValid{ lefts.subspan(1) };
    const auto rightsValid{ rights.subspan(1) };

    size_t numOfCollided{ 0 };
    std::array<uint64_t, 256> collided;

    constexpr float maxDistForCollision{ 15.f };
    constexpr float maxDistForCollisionSqr{ maxDistForCollision * maxDistForCollision };

    for (uint32_t leftIdx{ 0 }; leftIdx < leftsValid.size(); ++leftIdx)
    {
        for (uint32_t rightIdx{0}; rightIdx < rightsValid.size(); ++rightIdx)
        {
            const auto distanceSqr{ std::powf(rightsValid[rightIdx] - leftsValid[leftIdx], 2)};
            const auto bInRange{ distanceSqr <= maxDistForCollisionSqr };
            numOfCollided += size_t(1) * bInRange;
            const auto currentInTableId{ numOfCollided * bInRange };
            collided[currentInTableId] = (static_cast<uint64_t>(leftIdx) << 32) | rightIdx;

        }
    }

    return std::vector<uint64_t>(collided.begin() + 1, collided.begin() + numOfCollided + 1);

}

void bulletsCollisionUpdate(Scene& scene)
{

    const auto xAxisCollided{ getAxisCollisionsList(gPlayerBulletXCoords, gEnemiesXCorrds) };
    const auto yAxisCollided{ getAxisCollisionsList(gPlayerBulletYCoords, gEnemiesYCorrds) };

    std::vector<uint64_t> collided;
    collided.reserve(xAxisCollided.size() + yAxisCollided.size());
    std::ranges::set_intersection(xAxisCollided, yAxisCollided, std::back_inserter(collided));
    
    std::vector<int32_t> bulletsToDestroy;
 
    for (const auto collidedPair : collided)
        bulletsToDestroy.push_back(static_cast<int32_t>(collidedPair >> 32));
 
    for (const auto collidedPair : collided)
        gEnemyIdsToRemove.push_back(collidedPair + 1);
 
    for (const auto bulletIdToDestroy : bulletsToDestroy)
        Dod::BufferUtils::populate(scene.playerBulletIdsToRemove, bulletIdToDestroy + 1, true);

}

void bulletsMovementUpdate(const std::span<float> bulletY, float dt, float bulletVelocity)
{
    for (size_t idx{ 1 }; idx < bulletY.size(); ++idx) {
        bulletY[idx] += bulletVelocity * dt;
    }
}

void bulletsLifetimeUpdate(std::vector<float>& bulletsX, std::vector<float>& bulletsY, Dod::DBBuffer<int32_t>& bulletIdsToRemove)
{

    std::sort(bulletIdsToRemove.dataBegin + 1, bulletIdsToRemove.dataEnd, std::greater());
    Dod::Algorithms::leftUniques(bulletIdsToRemove);

    const auto applyRemoval = [&](auto& bufferRemoveFrom) -> void {
        size_t targetIdx{ bufferRemoveFrom.size() - 1 };
        for (size_t idx{ 0 }; idx < bulletIdsToRemove.numOfFilledEls; ++idx)
        {
            const auto removeId{ Dod::BufferUtils::get(bulletIdsToRemove, idx) };
            std::swap(bufferRemoveFrom[removeId], bufferRemoveFrom[targetIdx]);
            --targetIdx;
        }
        bufferRemoveFrom.resize(bufferRemoveFrom.size() - bulletIdsToRemove.numOfFilledEls);
    };

    applyRemoval(bulletsX);
    applyRemoval(bulletsY);

    bulletIdsToRemove.numOfFilledEls = 0;
}

void bulletsPlaneCollision(const std::span<const float> bulletsY, Dod::DBBuffer<int32_t>& bulletsToRemove, float planeY, float dir)
{

    size_t removeListSize{ 0 };
    std::array<int32_t, 255> removeList;
    for (uint32_t bulletId{ 1 }; bulletId < bulletsY.size(); ++bulletId)
    {
        const auto bIsColliding{ bulletsY[bulletId] * dir < planeY * dir };
        removeListSize += size_t(1) * bIsColliding;
        removeList[removeListSize * bIsColliding] = bulletId;
    }

    for (uint32_t bulletId{ 1 }; bulletId < removeListSize + 1; ++bulletId)
        Dod::BufferUtils::populate(bulletsToRemove, removeList[bulletId], true);

}

[[nodiscard]] auto pointsAreasIntersection(
    const std::span<const float> pointsCenter,
    Dod::ImBuffer<float> areasCenter,
    const float areasWidth
)
{

    size_t numOfIntersections{ 0 };
    std::array<uint64_t, 255> intersections;

    for (uint32_t pointId{ 0 }; pointId < pointsCenter.size(); ++pointId)
    {
        const auto pointCenter{ pointsCenter[pointId] };
        for (uint32_t areaId{ 0 }; areaId < areasCenter.numOfFilledEls; ++areaId)
        {
            const auto areaCenter{ Dod::BufferUtils::get(areasCenter, areaId) };
            const auto areaWidth{ areasWidth };
            const auto areaHalfWidth{ areaWidth * 0.5f };
            const auto areaLeft{ areaCenter - areaHalfWidth };
            const auto areaRight{ areaCenter + areaHalfWidth };

            const auto bInArea{ (pointCenter >= areaLeft) && (pointCenter <= areaRight) };

            numOfIntersections += size_t(1) * bInArea;
            intersections[numOfIntersections * bInArea] = (static_cast<uint64_t>(pointId) << 32) | areaId;
        }
    }

    return std::vector<uint64_t>(intersections.begin() + 1, intersections.begin() + 1 + numOfIntersections);

}

[[nodiscard]] auto pointsSquaresIntersection(
    const std::span<const float> pointsX,
    const std::span<const float> pointsY,
    Dod::ImBuffer<float> areasX,
    Dod::ImBuffer<float> areasY,
    const float areasWidth,
    const float areasHeight
)
{

    const auto intersectionsX{ pointsAreasIntersection(pointsX, areasX, areasWidth) };
    const auto intersectionsY{ pointsAreasIntersection(pointsY, areasY, areasHeight) };

    std::vector<uint64_t> intersections;
    intersections.reserve(intersectionsX.size() + intersectionsY.size());
    std::ranges::set_intersection(intersectionsX, intersectionsY, std::back_inserter(intersections));

    return intersections;

}

void testBulletsWithObstacles(
    const std::span<const float> bulletsX,
    const std::span<const float> bulletsY,
    Dod::DBBuffer<int32_t>& bulletsToDelete,
    Scene& scene
)
{

    Dod::ImBuffer<float> obstaclesX;
    Dod::BufferUtils::initFromBuffer(obstaclesX, scene.obstaclesX, 0, scene.obstaclesX.numOfFilledEls);
    Dod::ImBuffer<float> obstaclesY;
    Dod::BufferUtils::initFromBuffer(obstaclesY, scene.obstaclesY, 0, scene.obstaclesY.numOfFilledEls);

    const auto intersections{ pointsSquaresIntersection(
        bulletsX.subspan(1),
        bulletsY.subspan(1),
        obstaclesX,
        obstaclesY,
        Scene::obstaclesStride,
        Scene::obstaclesStride
    ) };

    for (const auto intersection : intersections)
        Dod::BufferUtils::populate(bulletsToDelete, static_cast<int32_t>(intersection >> 32) + 1, true);

    for (const auto intersection : intersections)
        Dod::BufferUtils::populate(scene.obstaclesToHit, static_cast<int32_t>(intersection), true);

}

[[nodiscard]] auto pointsAreaIntersection(
    const std::span<const float> pointsCenter,
    const float areaCenter,
    const float areaWidth
)
{

    size_t numOfIntersections{ 0 };
    std::array<uint64_t, 255> intersections;

    for (uint32_t pointId{ 0 }; pointId < pointsCenter.size(); ++pointId)
    {
        const auto pointCenter{ pointsCenter[pointId] };

        const auto areaHalfWidth{ areaWidth * 0.5f };
        const auto areaLeft{ areaCenter - areaHalfWidth };
        const auto areaRight{ areaCenter + areaHalfWidth };

        const auto bInArea{ (pointCenter >= areaLeft) && (pointCenter <= areaRight) };

        numOfIntersections += size_t(1) * bInArea;
        intersections[numOfIntersections * bInArea] = pointId;
    }

    return std::vector<uint32_t>(intersections.begin() + 1, intersections.begin() + 1 + numOfIntersections);

}

[[nodiscard]] auto pointsSquareIntersection(
    const std::span<const float> pointsX,
    const std::span<const float> pointsY,
    const float areaX,
    const float areaY,
    const float areaWidth,
    const float areaHeight
)
{

    const auto intersectionsX{ pointsAreaIntersection(pointsX, areaX, areaWidth) };
    const auto intersectionsY{ pointsAreaIntersection(pointsY, areaY, areaHeight) };

    std::vector<uint64_t> intersections;
    intersections.reserve(intersectionsX.size() + intersectionsY.size());
    std::ranges::set_intersection(intersectionsX, intersectionsY, std::back_inserter(intersections));

    return intersections;

}

[[nodiscard]] auto testBulletsWithPlayer(
    const std::span<const float> bulletsX,
    const std::span<const float> bulletsY,
    Dod::DBBuffer<int32_t>& bulletsToDelete,
    const float playerX,
    const float playerY,
    const int32_t playerLifes
)
{

    const auto intersections{ pointsSquareIntersection(
        bulletsX.subspan(1),
        bulletsY.subspan(1),
        playerX,
        playerY,
        60,
        30
    ) };

    for (const auto intersection : intersections)
        Dod::BufferUtils::populate(bulletsToDelete, static_cast<int32_t>(intersection) + 1, true);

    return playerLifes - int32_t(1) * !intersections.empty();

}

void testBulletsWithObstacles(
    const std::span<const float> bulletsX,
    const std::span<const float> bulletsY,
    std::vector<uint32_t>& bulletsToDelete,
    Dod::ImBuffer<float> areasX,
    Dod::ImBuffer<float> areasY,
    const float areasWidth,
    const float areasHeight,
    std::vector<size_t>& obstaclesToHit
)
{

    const auto intersections{ pointsSquaresIntersection(
        bulletsX.subspan(1),
        bulletsY.subspan(1),
        areasX,
        areasY,
        areasWidth,
        areasHeight
    ) };

    for (const auto intersection : intersections)
        bulletsToDelete.push_back((intersection >> 32) + 1);

    for (const auto intersection : intersections)
        obstaclesToHit.push_back(intersection + 1);

}

static sf::Vector2f gPlayerPosition;
static int32_t gPlayerLives{ 1 };
static int32_t prevInputBits;

void playerUpdate(float dt)
{

    const auto inputBits{ gatherInputs() };
//    const auto fakeInputBits{ prevInputBits | inputBits };
    
    const auto controlsState{ inputsUpdate(gCurrentControlState, inputBits, prevInputBits) };
    prevInputBits = inputBits;

    gCurrentControlState = controlsState;

    gPlayerPosition.x += controlsState.movementComponent * 200.f * dt;

    const auto bNeedCreateBullet{ (controlsState.fireComponent > 0) && (gPlayerLives > 0)};

    const auto numOfPlayerBullets{ gPlayerBulletXCoords.size() - 1 };
    const auto newBulletIdx{ bNeedCreateBullet * (numOfPlayerBullets + 1) };

    gPlayerBulletXCoords.resize(1 + numOfPlayerBullets + 1 * bNeedCreateBullet);
    gPlayerBulletYCoords.resize(1 + numOfPlayerBullets + 1 * bNeedCreateBullet);

    gPlayerBulletXCoords[newBulletIdx] = gPlayerPosition.x;
    gPlayerBulletYCoords[newBulletIdx] = gPlayerPosition.y;

}

auto generateEnemies(float rootX, float rootY, size_t enemiesInRow, size_t numOfCols)
{

    std::vector<float> enemiesXCoords{0.f};
    std::vector<float> enemiesYCoords{0.f};

    const auto totalEnemies{ enemiesInRow * numOfCols };
    enemiesXCoords.reserve(totalEnemies + 1);
    enemiesYCoords.reserve(totalEnemies + 1);

    for (size_t enemyId{ 0 }; enemyId < totalEnemies; ++enemyId)
    {
        const auto inRowId{ enemyId % enemiesInRow };
        const auto xPosition{ rootX + inRowId * enemiesXStride };
        enemiesXCoords.push_back(xPosition);
    }

    for (size_t enemyId{ 0 }; enemyId < totalEnemies; ++enemyId)
    {
        const auto inColId{ enemyId / enemiesInRow };
        const auto yPosition{ rootY + inColId * enemiesYStride };
        enemiesYCoords.push_back(yPosition);
    }

    struct Output
    {
        std::vector<float> enemiesXCoords;
        std::vector<float> enemiesYCoords;
    };

    return Output(std::move(enemiesXCoords), std::move(enemiesYCoords));

}

void obstaclesLifetimeUpdate(Scene& scene)
{

    std::array<int32_t, 256> obstaclesToRemoveMemory;
    Dod::DBBuffer<int32_t> obstaclesToRemove;
    struct MemSpan
    {
        Dod::MemTypes::dataPoint_t dataBegin{};
        Dod::MemTypes::dataPoint_t dataEnd{};
    };
    const MemSpan memSpan(
        reinterpret_cast<Dod::MemTypes::dataPoint_t>(obstaclesToRemoveMemory.data()),
        reinterpret_cast<Dod::MemTypes::dataPoint_t>(obstaclesToRemoveMemory.data() + obstaclesToRemoveMemory.size())
    );
    Dod::BufferUtils::initFromMemory(obstaclesToRemove, memSpan, 0, obstaclesToRemoveMemory.size());

    for (int32_t elId{ 0 }; elId < scene.obstaclesToHit.numOfFilledEls; ++elId)
    {
        const auto obstacleToHitId{ Dod::BufferUtils::get(scene.obstaclesToHit, elId) };
        const auto lifesLeft{ --Dod::BufferUtils::get(scene.obstaclesLifes, obstacleToHitId)};
        const auto bNeedToRemove{ lifesLeft == 0 };

        Dod::BufferUtils::populate(obstaclesToRemove, obstacleToHitId, bNeedToRemove);
    }

    Dod::ImBuffer<int32_t> obstaclesToRemoveIm;
    Dod::BufferUtils::initFromBuffer(obstaclesToRemoveIm, obstaclesToRemove, 0, obstaclesToRemove.numOfFilledEls);

    Dod::BufferUtils::remove(scene.obstaclesX, obstaclesToRemoveIm);
    Dod::BufferUtils::remove(scene.obstaclesY, obstaclesToRemoveIm);
    Dod::BufferUtils::remove(scene.obstaclesLifes, obstaclesToRemoveIm);

    scene.obstaclesToHit.numOfFilledEls = 0;

}

void generateObstacles(Scene& scene)
{

//    std::vector<float> obstaclesX{ 0.f };
//    std::vector<float> obstaclesY{ 0.f };
//    std::vector<uint32_t> obstacleLifes{ 0 };

    const auto totalObstaclesPerCluster{ Scene::obstaclesPerRow * Scene::obstaclesPerCol };
    const auto totalObstacles{ totalObstaclesPerCluster * Scene::obstaclesClusters };

//    obstaclesX.reserve(size_t(1) + totalObstacles);
//    obstaclesY.reserve(size_t(1) + totalObstacles);
//    obstaclesY.reserve(size_t(1) + totalObstacles);
//    obstacleLifes.reserve(size_t(1) + totalObstacles);

    const auto widthPerCluster{ Scene::obstaclesPerRow * Scene::obstaclesStride };
    const auto totalObstaclesWidth{ widthPerCluster * Scene::obstaclesClusters };
    const auto needObstaclesWidth{ Scene::obstaclesClustersTotalAreaWidth };
    const auto totalGapWidth{ needObstaclesWidth - totalObstaclesWidth };
    const auto numOfClusterGaps{ Scene::obstaclesClusters - 1 };
    const auto gapWidth{ totalGapWidth / numOfClusterGaps };
    const auto distanceBetweenClusters{ gapWidth + widthPerCluster };

    const auto obstaclesStartPositionX{ windowWidth * 0.5f - totalObstaclesWidth };

    const auto obstaclesClusterY{ 650.f };

    const auto initialClusterX{ windowWidth * 0.5f - needObstaclesWidth * 0.5f };

    for (int32_t clusterId{ 0 }; clusterId < Scene::obstaclesClusters; ++clusterId)
    {
        const auto obstaclesClusterX{ initialClusterX + distanceBetweenClusters * clusterId };
        for (int32_t obstacleId{ 0 }; obstacleId < totalObstaclesPerCluster; ++obstacleId)
        {
            const auto colId{ obstacleId % Scene::obstaclesPerRow };
            const auto rowId{ obstacleId / Scene::obstaclesPerRow };

            const auto x{ obstaclesClusterX + colId * Scene::obstaclesStride };
            const auto y{ obstaclesClusterY + rowId * Scene::obstaclesStride };

            Dod::BufferUtils::populate(scene.obstaclesX, x, true);
            Dod::BufferUtils::populate(scene.obstaclesY, y, true);
            Dod::BufferUtils::populate(scene.obstaclesLifes, 1, true);

//            obstaclesX.push_back(x);
//            obstaclesY.push_back(y);
//            obstacleLifes.push_back(3);
        }
    }
     
}

void msgLoop(sf::RenderWindow& window, float dt, Scene& scene)
{

    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    window.clear();

    playerUpdate(dt);
    bulletsMovementUpdate(gPlayerBulletYCoords, dt, -220.f);
    //bulletsUpdate(dt);
    bulletsCollisionUpdate(scene);
    bulletsPlaneCollision(gPlayerBulletYCoords, scene.playerBulletIdsToRemove, 100.f, 1.f);
    testBulletsWithObstacles(
        gPlayerBulletXCoords,
        gPlayerBulletYCoords,
        scene.playerBulletIdsToRemove,
        scene
    );

    bulletsLifetimeUpdate(gPlayerBulletXCoords, gPlayerBulletYCoords, scene.playerBulletIdsToRemove);

    enemiesLifetimeUpdate();
    const auto enemiesResult {enemiesUpdate(
        dt, 
        gEnemiesDirection, 
        gEnemiesBatchTargetX, 
        gEnemiesBatchCoordX, 
        gEnemiesBatchCoordY, 
        gEnemiesXCorrds, 
        gEnemiesYCorrds
    ) };
    gEnemiesDirection = enemiesResult.batchDirection;
    gEnemiesBatchTargetX = enemiesResult.batchTargetX;
    gEnemiesBatchCoordX = enemiesResult.batchXCoord;
    gEnemiesBatchCoordY = enemiesResult.batchYCoord;

    generateEnemyBullets(
        gEnemiesXCorrds,
        gEnemiesYCorrds,
        gEnemyBulletXCoords,
        gEnemyBulletYCoords,
        generateEnemyBulletRule(dt, gEnemyBulletDelayTimeLeft)
    );
    bulletsMovementUpdate(gEnemyBulletYCoords, dt, 120.f);

    bulletsPlaneCollision(gEnemyBulletYCoords, scene.enemyBulletIdsToRemove, 875.f, -1.f);
    gPlayerLives = testBulletsWithPlayer(gEnemyBulletXCoords, gEnemyBulletYCoords, scene.enemyBulletIdsToRemove, gPlayerPosition.x, gPlayerPosition.y, gPlayerLives);
    testBulletsWithObstacles(
        gEnemyBulletXCoords,
        gEnemyBulletYCoords,
        scene.enemyBulletIdsToRemove,
        scene
    );
    bulletsLifetimeUpdate(gEnemyBulletXCoords, gEnemyBulletYCoords, scene.enemyBulletIdsToRemove);

    obstaclesLifetimeUpdate(scene);

    drawEnemies(window, gEnemiesXCorrds, gEnemiesYCorrds);

    drawObstacles(window, scene);

    drawField(window);
    drawPlayer(window, gPlayerPosition, gPlayerLives);

    drawBullets(window, gPlayerBulletXCoords, gPlayerBulletYCoords);
    drawBullets(window, gEnemyBulletXCoords, gEnemyBulletYCoords);

    window.display();

}

void Game::run()
{

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "dod Space Action");
    gPlayerPosition = sf::Vector2f(400.f, 850.f);

    gPlayerBulletXCoords.emplace_back();
    gPlayerBulletYCoords.emplace_back();

    gEnemiesBatchCoordX = 100.f;
    gEnemiesBatchCoordY = 200.f;

    const auto enemies{ generateEnemies(gEnemiesBatchCoordX, gEnemiesBatchCoordY, numOfEnemiesPerRow, numOfEnemiesCols) };
    gEnemiesXCorrds = enemies.enemiesXCoords;
    gEnemiesYCorrds = enemies.enemiesYCoords;

    gEnemyBulletXCoords.emplace_back();
    gEnemyBulletYCoords.emplace_back();

    Scene scene;

    generateObstacles(scene);
//    gObstaclesX = obstacles.obstaclesXCoords;
//    gObstaclesY = obstacles.obstaclesYCoords;
//    gObstacleLifes = obstacles.obstacleLifes;


    
    float deltaTime{ 0.f };

    while (window.isOpen())
    {
        const auto start{ std::chrono::high_resolution_clock::now() };
        msgLoop(window, deltaTime, scene);
        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1'000'000'000.f;
    }

}
