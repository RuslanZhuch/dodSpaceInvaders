#pragma once

#include <dod/Buffers.h>

namespace Game::Context::Obstacles
{

    struct Parameters
    {
        float obstaclesStride{ };
        int32_t obstaclesPerRow{ };
        int32_t obstaclesPerCol{ };
        int32_t obstaclesClusters{ };
        int32_t obstaclesClustersTotalAreaWidth{ };
        int32_t obstaclesClusterY{ };
        int32_t initialLifes{ };
    };

    struct UnitsContext
    {
        Dod::DBBuffer<float> xCoords;
        Dod::DBBuffer<float> yCoords;
        Dod::DBBuffer<int32_t> lifes;
        Dod::DBBuffer<int32_t> toHit;
        Dod::DBBuffer<int32_t> toRemove;
    };

}
