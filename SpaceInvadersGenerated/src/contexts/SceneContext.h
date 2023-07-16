#pragma once

#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::Scene
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        float topPlaneY{};
        float topPlaneDir{};
        float padding{};
        float bottomPlaneY{};
        float bottomPlaneDir{};
        Dod::MemPool memory;
    };
}
