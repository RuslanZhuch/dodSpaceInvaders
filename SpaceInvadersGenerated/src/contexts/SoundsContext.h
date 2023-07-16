#pragma once

#include <internal/sound/soundCore.h>
#include <internal/sound/soundCore.h>
#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::Sounds
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        Game::Sound::Core soundsCore;
        Dod::MemPool memory;
        Dod::DBBuffer<Game::Sound::Wav> sounds;
    };
}
