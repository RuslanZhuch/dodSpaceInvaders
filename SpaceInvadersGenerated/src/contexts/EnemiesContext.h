#pragma once

#include <internal/utils/randomGenerator.h>
#include <dod/Buffers.h>
#include <dod/MemPool.h>

namespace Game::Context::Enemies
{
    struct Data
    {
        void load() noexcept;
        void reset() noexcept;
        void merge(const Data& other) noexcept;

        int32_t numOfEnemiesPerRow{};
        int32_t numOfEnemiesPerCol{};
        float enemiesXStride{};
        float enemiesYStride{};
        float weaponCooldownTime{};
        float weaponCooldownTimeLeft{};
        float startCoordX{};
        float startCoordY{};
        float direction{};
        float cooldownTimeLeft{};
        float cooldownTime{};
        float targetX{};
        Game::Utils::RandomGenerator rand;
        Dod::MemPool memory;
    };
}
