#pragma once

#include <internal/render/Renderer.h>
#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::ModelsToRender
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        Dod::MemPool memory;
        Dod::DBBuffer<sf::VertexArray> loadedModels;
        Dod::DBBuffer<int32_t> modelIds;
    };
}
