#pragma once

#include <internal/render/Renderer.h>
#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::RenderInternal
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        Game::GameRendererPtr renderer{};
        Dod::MemPool memory;
        Dod::DBBuffer<int32_t> idsToRender;
    };
}
