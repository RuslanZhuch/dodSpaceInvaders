#pragma once

#include <dod/Buffers.h>
#include <dod/MemPool.h>

#include <array>

namespace Game::Context::Common
{

    struct Data
    {

        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        Dod::MemPool memory;

        float width{};
        float height{};
        std::array<char, 64> title{};

    };

};
