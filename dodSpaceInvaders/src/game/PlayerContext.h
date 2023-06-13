#pragma once

#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::Player
{

    struct Data
    {

        [[nodiscard]] static Data load(Dod::MemPool& pool, int32_t& header) noexcept;

        float width{};
        float height{};
        float xCoord{};
        float yCoord{};
        int32_t lifes{};
        uint32_t inputs{};
        uint32_t prevInputs{};
        float move{};
        float fire{};

    };

};
