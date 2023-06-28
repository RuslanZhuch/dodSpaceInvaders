#pragma once

#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::PlayerDimentions
{

    struct Data
    {

        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        float width{};
        float height{};

    };

};
