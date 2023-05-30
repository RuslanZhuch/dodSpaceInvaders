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
        float width{};
    };

    struct UnitsContext
    {
        Dod::DBBuffer<int32_t> lifes;
        Dod::DBBuffer<int32_t> toRemove;
    };

}
