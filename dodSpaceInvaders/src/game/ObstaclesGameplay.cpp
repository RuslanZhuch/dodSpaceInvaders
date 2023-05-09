#include "ObstaclesGameplay.h"

#include <cinttypes>

void Game::Gameplay::Obstacles::create(
    Dod::DBBuffer<float>& obstaclesX,
    Dod::DBBuffer<float>& obstaclesY,
    Dod::DBBuffer<int32_t>& obstaclesLifes,
    int32_t obstaclesPerRow,
    int32_t obstaclesPerCol,
    float obstaclesStride,
    int32_t obstaclesClusters,
    float obstaclesClustersTotalAreaWidth,
    float obstaclesClusterY,
    int32_t initialLifes,
    float windowWidth
) noexcept
{


    const auto totalObstaclesPerCluster{ obstaclesPerRow * obstaclesPerCol };

    const auto widthPerCluster{ obstaclesPerRow * obstaclesStride };
    const auto totalObstaclesWidth{ widthPerCluster * obstaclesClusters };
    const auto needObstaclesWidth{ obstaclesClustersTotalAreaWidth };
    const auto totalGapWidth{ needObstaclesWidth - totalObstaclesWidth };
    const auto numOfClusterGaps{ obstaclesClusters - 1 };
    const auto gapWidth{ totalGapWidth / numOfClusterGaps };
    const auto distanceBetweenClusters{ gapWidth + widthPerCluster };

//    const auto obstaclesClusterY{ 650.f };

    const auto initialClusterX{ windowWidth * 0.5f - needObstaclesWidth * 0.5f };

    for (int32_t clusterId{ 0 }; clusterId < obstaclesClusters; ++clusterId)
    {
        const auto obstaclesClusterX{ initialClusterX + distanceBetweenClusters * static_cast<float>(clusterId) };
        for (int32_t obstacleId{ 0 }; obstacleId < totalObstaclesPerCluster; ++obstacleId)
        {
            const auto colId{ obstacleId % obstaclesPerRow };
            const auto rowId{ obstacleId / obstaclesPerRow };

            const auto x{ obstaclesClusterX + static_cast<float>(colId) * obstaclesStride };
            const auto y{ obstaclesClusterY + static_cast<float>(rowId) * obstaclesStride };

            Dod::BufferUtils::populate(obstaclesX, x, true);
            Dod::BufferUtils::populate(obstaclesY, y, true);
            Dod::BufferUtils::populate(obstaclesLifes, initialLifes, true);
        }
    }


}
