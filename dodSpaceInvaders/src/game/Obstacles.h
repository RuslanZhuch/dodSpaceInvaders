#pragma once

#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::Obstacles
{

    struct Data
    {

        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        float obstaclesStride{ };
        int32_t obstaclesPerRow{ };
        int32_t obstaclesPerCol{ };
        int32_t obstaclesClusters{ };
        int32_t obstaclesClustersTotalAreaWidth{ };
        int32_t obstaclesClusterY{ };
        int32_t initialLifes{ };
        float width{};

        Dod::MemPool memory;
        Dod::DBBuffer<int32_t> lifes;
        Dod::DBBuffer<int32_t> toRemove;
    };

}
