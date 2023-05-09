#pragma once

#include <dod/Buffers.h>

namespace Game::Context::Player
{

    struct Parameters
    {
        float width{};
        float height{};
    };

    struct Position
    {
        float xCoord{};
        float yCoord{};
    };

    struct Lifetime
    {
        int32_t lifes{};
    };

    struct Inputs
    {
        uint32_t inputs{};
        uint32_t prevInputs{};
    };

    struct Movement
    {
        float move{};
    };

    struct Fire
    {
        float move{};
    };

};
