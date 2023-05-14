#pragma once

#include <dod/Buffers.h>

namespace Game::Context::Bullets
{

    struct Parameters
    {
        float velocity{};
    };

    struct UnitsContext
    {
        Dod::DBBuffer<float> xCoords;
        Dod::DBBuffer<float> yCoords;
        Dod::DBBuffer<int32_t> toRemove;
    };


}
