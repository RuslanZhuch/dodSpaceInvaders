#pragma once

#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::Player
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        float positionX{};
        float positionY{};
        int32_t lifes{};
        uint32_t inputs{};
        uint32_t prevInputs{};
        float move{};
        int32_t fire{};
        Dod::MemPool memory;
    };
}
