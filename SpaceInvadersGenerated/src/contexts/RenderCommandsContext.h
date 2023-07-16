#pragma once

#include <internal/modelTypes.h>
#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::RenderCommands
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        Dod::MemPool memory;
        Dod::DBBuffer<float> xCoords;
        Dod::DBBuffer<float> yCoords;
        Dod::DBBuffer<Models::ModelMeta> modelsMeta;
    };
}
