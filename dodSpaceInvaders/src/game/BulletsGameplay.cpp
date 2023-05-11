#include "BulletsGameplay.h"

#include "game/CollisionCore.h"

#include <dod/Algorithms.h>

#include <array>

void Game::Gameplay::Bullets::collisionUpdate(
    Dod::DBBuffer<int32_t>& enemiesToRemove,
    Dod::DBBuffer<int32_t>& playerBulletsToRemove,
    Dod::ImBuffer<float> playersBulletXCoords,
    Dod::ImBuffer<float> playersBulletYCoords,
    Dod::ImBuffer<float> enemiesXCoords,
    Dod::ImBuffer<float> enemiesYCoords,
    float enemiesWidth,
    float enemiesHeight
) noexcept
{

    std::array<uint64_t, 128> xAxisCollidedMem;
    Dod::DBBuffer<uint64_t> xAxisCollided;
    Dod::BufferUtils::initFromArray(xAxisCollided, xAxisCollidedMem);

    std::array<uint64_t, 128> yAxisCollidedMem;
    Dod::DBBuffer<uint64_t> yAxisCollided;
    Dod::BufferUtils::initFromArray(yAxisCollided, yAxisCollidedMem);

    Game::Core::Collision::pointsAreasIntersection(xAxisCollided, playersBulletXCoords, enemiesXCoords, enemiesWidth);
    Game::Core::Collision::pointsAreasIntersection(yAxisCollided, playersBulletYCoords, enemiesYCoords, enemiesHeight);

    std::array<uint64_t, 512> collidedMem;
    Dod::DBBuffer<uint64_t> collided;
    Dod::BufferUtils::initFromArray(collided, collidedMem);

    Dod::Algorithms::getIntersections(collided, xAxisCollided, yAxisCollided);

    for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(collided); ++elId)
        Dod::BufferUtils::populate(enemiesToRemove, static_cast<int32_t>(Dod::BufferUtils::get(collided, elId)), true);

    for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(collided); ++elId)
        Dod::BufferUtils::populate(playerBulletsToRemove, static_cast<int32_t>(Dod::BufferUtils::get(collided, elId) >> 32), true);

}

void Game::Gameplay::Bullets::testWithObstacles(
    Dod::DBBuffer<int32_t>& bulletsToDelete,
    Dod::DBBuffer<int32_t>& obstaclesToHit,
    Dod::ImBuffer<float> bulletsX,
    Dod::ImBuffer<float> bulletsY,
    Dod::ImBuffer<float> obstaclesX,
    Dod::ImBuffer<float> obstaclesY,
    float obstaclesWidth,
    float obstaclesHeight
) noexcept
{

    std::array<uint64_t, 128> intersectionsMem;
    Dod::DBBuffer<uint64_t> intersections;
    Dod::BufferUtils::initFromArray(intersections, intersectionsMem);

    Game::Core::Collision::pointsSquaresIntersection(
        intersections,
        bulletsX,
        bulletsY,
        obstaclesX,
        obstaclesY,
        obstaclesWidth,
        obstaclesHeight
    );

    for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(intersections); ++elId)
        Dod::BufferUtils::populate(obstaclesToHit, static_cast<int32_t>(Dod::BufferUtils::get(intersections, elId)), true);

    for (int32_t elId{}; elId < Dod::BufferUtils::getNumFilledElements(intersections); ++elId)
        Dod::BufferUtils::populate(bulletsToDelete, static_cast<int32_t>(Dod::BufferUtils::get(intersections, elId) >> 32), true);

}
