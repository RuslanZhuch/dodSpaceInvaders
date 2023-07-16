#pragma once

#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::PlayerBullets
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        float velocity{};
        Dod::MemPool memory;
    };
}
