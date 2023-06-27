#pragma once

#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::ObstacleParameters
{

    struct Data
    {

        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        float width{};
        float height{};

        Dod::MemPool memory;

    };

}
