
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

constexpr auto numOfCells{ 20 };
constexpr auto filedSizeCoeff{ 0.95f };
constexpr auto numOfEnemiesPerRow{ 10 };
constexpr auto numOfEnemiesCols{ 4 };
constexpr float enemiesXStride{ 50.f };
constexpr float enemiesYStride{ 50.f };

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

void drawPlayer(sf::RenderWindow& window, const sf::Vector2f& position)
{

    constexpr auto radius{ 25.f };
    const auto leftPoint{ position - sf::Vector2f(-radius, 0.f) };
    const auto rightPoint{ position - sf::Vector2f(radius, 0.f) };
    const auto topPoint{ position - sf::Vector2f(0.f, radius) };

    const auto playerColor{ sf::Color(150.f, 200.f, 90.f) };
    Renderer::Instant::drawLine(window, leftPoint, rightPoint, playerColor);
    Renderer::Instant::drawLine(window, leftPoint, topPoint, playerColor);
    Renderer::Instant::drawLine(window, rightPoint, topPoint, playerColor);

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
    {
//        std::cout << std::format("Render enemy {}, {}\n", x, y);
        drawEnemy(window, { x, y });
    }
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

//size_t gNumOfPlayerBullets{ 0 };

static std::random_device gRandomDevice;
static std::mt19937 gRandomGen(gRandomDevice());

std::vector<float> gPlayerBulletXCoords;
std::vector<float> gPlayerBulletYCoords;
std::vector<uint32_t> gPlayerBulletIdsToRemove;

std::vector<float> gEnemyBulletXCoords;
std::vector<float> gEnemyBulletYCoords;
std::vector<uint32_t> gEnemyBulletIdsToRemove;

size_t gNumOfRemovedBullets{ 0 };
std::vector<size_t> gRemovedBulletIds;

std::vector<float> gEnemiesXCorrds;
std::vector<float> gEnemiesYCorrds;
std::vector<uint32_t> gEnemyIdsToRemove;

float gEnemiesBatchTargetX{};
float gEnemiesBatchCoordX{};
float gEnemiesBatchCoordY{};
float gEnemiesBatchMoveTimeleft{};
float gEnemiesDirection = 1;

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

//    std::cout << std::format("Inputs bitmask {:b}\n", inputs);

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

auto generateEnemyBulletRule()
{

    std::uniform_int_distribution<> distrib(0, 1000);
    const auto randValue{ distrib(gRandomGen) };
    const auto bNeedCreateBullet{ randValue > 990 };

    return bNeedCreateBullet;

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
//            std::cout << std::format("bullet pos {} id {}, en pos {} id {}, distance {}, in range {}\n",
//                leftsValid[leftIdx],
//                leftIdx,
//                rightsValid[rightIdx],
//                rightIdx,
//                std::sqrtf(distanceSqr),
//                bInRange
//            );
            collided[currentInTableId] = (static_cast<uint64_t>(leftIdx) << 32) | rightIdx;

        }
    }

    return std::vector<uint64_t>(collided.begin() + 1, collided.begin() + numOfCollided + 1);

}

void bulletsCollisionUpdate()
{
//    std::cout << "x\n";
    const auto xAxisCollided{ getAxisCollisionsList(gPlayerBulletXCoords, gEnemiesXCorrds) };
//    std::cout << "y\n";
    const auto yAxisCollided{ getAxisCollisionsList(gPlayerBulletYCoords, gEnemiesYCorrds) };

    const auto minSize{ std::min(xAxisCollided.size(), yAxisCollided.size())};
    std::vector<uint64_t> collided;
    collided.reserve(xAxisCollided.size() + yAxisCollided.size());
    std::ranges::set_intersection(xAxisCollided, yAxisCollided, std::back_inserter(collided));
    
    std::vector<uint32_t> bulletsToDestroy;
 
    for (const auto collidedPair : collided)
        bulletsToDestroy.push_back(collidedPair >> 32);
 
    for (const auto collidedPair : collided)
        gEnemyIdsToRemove.push_back(collidedPair + 1);
 
    for (const auto bulletIdToDestroy : bulletsToDestroy)
        gPlayerBulletIdsToRemove.push_back(bulletIdToDestroy + 1);

}

void bulletsMovementUpdate(const std::span<float> bulletY, float dt, float bulletVelocity)
{
    for (size_t idx{ 1 }; idx < bulletY.size(); ++idx) {
        bulletY[idx] += bulletVelocity * dt;
    }
}

void bulletsLifetimeUpdate(std::vector<float>& bulletsX, std::vector<float>& bulletsY, std::vector<uint32_t>& bulletIdsToRemove)
{
    const auto applyRemoval = [&](auto& bufferRemoveFrom) -> void {
        size_t targetIdx{ bufferRemoveFrom.size() - 1 };
        for (size_t idx{ 0 }; idx < bulletIdsToRemove.size(); ++idx)
        {
            const auto removeId{ bulletIdsToRemove[idx] };
            std::swap(bufferRemoveFrom[removeId], bufferRemoveFrom[targetIdx]);
            --targetIdx;
        }
        bufferRemoveFrom.resize(bufferRemoveFrom.size() - bulletIdsToRemove.size());
    };

    applyRemoval(bulletsX);
    applyRemoval(bulletsY);

    bulletIdsToRemove.clear();
}

void bulletsPlaneCollision(const std::span<const float> bulletsY, std::vector<uint32_t>& bulletsToRemove, float planeY, float dir)
{

    size_t removeListSize{ 0 };
    std::array<uint32_t, 255> removeList;
    for (uint32_t bulletId{ 1 }; bulletId < bulletsY.size(); ++bulletId)
    {
        const auto bIsColliding{ bulletsY[bulletId] * dir < planeY * dir };
        removeListSize += size_t(1) * bIsColliding;
        removeList[removeListSize * bIsColliding] = bulletId;
    }

    for (uint32_t bulletId{ 1 }; bulletId < removeListSize + 1; ++bulletId)
        bulletsToRemove.push_back(removeList[bulletId]);

}

static sf::Vector2f gPlayerPosition;
static int32_t prevInputBits;

void playerUpdate(float dt)
{

    const auto inputBits{ gatherInputs() };
//    const auto fakeInputBits{ prevInputBits | inputBits };
    
    const auto controlsState{ inputsUpdate(gCurrentControlState, inputBits, prevInputBits) };
    prevInputBits = inputBits;

    gCurrentControlState = controlsState;

    gPlayerPosition.x += controlsState.movementComponent * 200.f * dt;

    const auto bNeedCreateBullet{ controlsState.fireComponent > 0 };

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

    return Output(enemiesXCoords, enemiesYCoords);

}

void msgLoop(sf::RenderWindow& window, float dt)
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
    bulletsCollisionUpdate();
    bulletsPlaneCollision(gPlayerBulletYCoords, gPlayerBulletIdsToRemove, 100.f, 1.f);
    bulletsLifetimeUpdate(gPlayerBulletXCoords, gPlayerBulletYCoords, gPlayerBulletIdsToRemove);

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
        generateEnemyBulletRule()
    );
    bulletsMovementUpdate(gEnemyBulletYCoords, dt, 120.f);

    bulletsPlaneCollision(gEnemyBulletYCoords, gEnemyBulletIdsToRemove, 800.f, -1.f);
    bulletsLifetimeUpdate(gEnemyBulletXCoords, gEnemyBulletYCoords, gEnemyBulletIdsToRemove);

    drawEnemies(window, gEnemiesXCorrds, gEnemiesYCorrds);

    drawField(window);
    drawPlayer(window, gPlayerPosition);

    drawBullets(window, gPlayerBulletXCoords, gPlayerBulletYCoords);
    drawBullets(window, gEnemyBulletXCoords, gEnemyBulletYCoords);

//    drawFood(window, food);
//    drawSnake(window, snake);

    window.display();

}

void Game::run()
{

    sf::RenderWindow window(sf::VideoMode(800, 900), "dod Space Action");
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
    
    float deltaTime{ 0.f };

    while (window.isOpen())
    {
        const auto start{ std::chrono::high_resolution_clock::now() };
        msgLoop(window, deltaTime);
        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1'000'000'000.f;
    }

}
