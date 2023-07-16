#pragma once

#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::SoundCmds
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        Dod::MemPool memory;
        Dod::DBBuffer<int32_t> soundIdsToPlay;
    };
}
