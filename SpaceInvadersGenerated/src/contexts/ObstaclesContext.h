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

        float stride{};
        int32_t perRow{};
        int32_t perCol{};
        int32_t numOfClusters{};
        float clustersTotalAreaWidth{};
        float clusterY{};
        int32_t initialLifes{};
        float width{};
        Dod::MemPool memory;
        Dod::DBBuffer<int32_t> lifes;
        Dod::DBBuffer<int32_t> toRemove;
    };
}
