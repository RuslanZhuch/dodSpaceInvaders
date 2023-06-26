#pragma once

#include <dod/MemPool.h>

namespace Game::Context::PlayerBullets
{

    struct Data
    {

        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        Dod::MemPool memory;
        float velocity{};

    };

}
