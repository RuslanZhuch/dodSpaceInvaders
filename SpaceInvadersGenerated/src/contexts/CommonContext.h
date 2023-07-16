#pragma once

#include <engine/String.h>
#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::Common
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        float width{};
        float height{};
        Engine::String title{};
        Dod::MemPool memory;
    };
}
