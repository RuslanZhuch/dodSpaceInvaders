#include "CollisionCore.h"

#include <dod/Algorithms.h>

#include <array>
#include <cmath>

void Game::Core::Collision::pointsAreaIntersection(
    Dod::DBBuffer<int32_t>& intersections,
    const Dod::ImBuffer<float>& pointsCenter,
    const float areaCenter,
    const float maxRadius
) noexcept
{

    const auto maxDistForCollisionSqr{ maxRadius * maxRadius };

    for (int32_t pointId{ 0 }; pointId < pointsCenter.numOfFilledEls; ++pointId)
    {
        const auto pointCenter{ Dod::BufferUtils::get(pointsCenter, pointId) };

        const auto distanceSqr{ std::powf(Dod::BufferUtils::get(pointsCenter, pointId) - areaCenter, 2) };
        const auto bInRange{ distanceSqr <= maxDistForCollisionSqr };

        Dod::BufferUtils::populate(intersections, pointId, bInRange);
    }

}

void Game::Core::Collision::pointsAreasIntersection(
	Dod::DBBuffer<uint64_t>& collisions, 
	const Dod::ImBuffer<float>& lefts, 
	const Dod::ImBuffer<float>& rights, 
	float maxRadius
) noexcept
{
    const auto maxDistForCollisionSqr{ maxRadius * maxRadius };

    for (int32_t leftIdx{ 0 }; leftIdx < lefts.numOfFilledEls; ++leftIdx)
    {
        for (int32_t rightIdx{ 0 }; rightIdx < rights.numOfFilledEls; ++rightIdx)
        {
            const auto distanceSqr{ std::powf(Dod::BufferUtils::get(rights, rightIdx) - Dod::BufferUtils::get(lefts, leftIdx), 2) };
            const auto bInRange{ distanceSqr <= maxDistForCollisionSqr };
            Dod::BufferUtils::populate(collisions, (static_cast<uint64_t>(leftIdx) << 32) | rightIdx, bInRange);
        }
    }
}

void Game::Core::Collision::pointsSquareIntersection(
    Dod::DBBuffer<int32_t>& intersections,
    const Dod::ImBuffer<float>& xPointsCoords,
    const Dod::ImBuffer<float>& yPointsCoords,
    float squareX,
    float squareY,
    float width,
    float height
) noexcept
{

    std::array<int32_t, 128> intersectionsXMem;
    Dod::DBBuffer<int32_t> intersectionsX;
    Dod::BufferUtils::initFromArray(intersectionsX, intersectionsXMem);

    std::array<int32_t, 128> intersectionsYMem;
    Dod::DBBuffer<int32_t> intersectionsY;
    Dod::BufferUtils::initFromArray(intersectionsY, intersectionsYMem);

    pointsAreaIntersection(intersectionsX, xPointsCoords, squareX, width);
    pointsAreaIntersection(intersectionsY, yPointsCoords, squareY, height);

    Dod::Algorithms::getIntersections(intersections, intersectionsX, intersectionsY);

}

void Game::Core::Collision::pointsSquaresIntersection(
    Dod::DBBuffer<uint64_t>& intersections, 
    const Dod::ImBuffer<float>& xPointsCoords, 
    const Dod::ImBuffer<float>& yPointsCoords, 
    const Dod::ImBuffer<float>& xSquaresCoords, 
    const Dod::ImBuffer<float>& ySquaresCoords, 
    float widths, 
    float heights
) noexcept
{

    constexpr size_t maxPerAxisIntersections{ 128 };

    std::array<uint64_t, maxPerAxisIntersections> intersectionsXMem;
    Dod::DBBuffer<uint64_t> intersectionsX;
    Dod::BufferUtils::initFromArray(intersectionsX, intersectionsXMem);

    std::array<uint64_t, maxPerAxisIntersections> intersectionsYMem;
    Dod::DBBuffer<uint64_t> intersectionsY;
    Dod::BufferUtils::initFromArray(intersectionsY, intersectionsYMem);

    pointsAreasIntersection(intersectionsX, xPointsCoords, xSquaresCoords, widths);
    pointsAreasIntersection(intersectionsY, yPointsCoords, ySquaresCoords, heights);

    Dod::Algorithms::getIntersections(intersections, intersectionsX, intersectionsY);

}

void Game::Core::Collision::pointsPlaneIntersection(
    Dod::DBBuffer<int32_t>& intersections, 
    const Dod::ImBuffer<float>& yPointsCoords, 
    float planeY, 
    float direction
) noexcept
{

    for (int32_t pointId{ 0 }; pointId < yPointsCoords.numOfFilledEls; ++pointId)
    {
        const auto bIsColliding{ Dod::BufferUtils::get(yPointsCoords, pointId) * direction <= planeY * direction };
        Dod::BufferUtils::populate(intersections, pointId, bIsColliding);
    }

}
