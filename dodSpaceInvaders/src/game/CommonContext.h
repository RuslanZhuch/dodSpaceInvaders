#pragma once

#include <dod/Buffers.h>

#include <array>

namespace Game::Context::Common
{

    struct Parameters
    {
        float width{};
        float height{};
        std::array<char, 64> title{};
    };

};
