#pragma once

#include <dod/Buffers.h>
#include <dod/MemPool.h>

#include <utils/randomGenerator.h>

namespace Game::Context::Enemy
{

    struct Data
    {

        int32_t numOfEnemiesPerRow{};
        int32_t numOfEnemiesCols{};
        float enemiesXStride{};
        float enemiesYStride{};
        float width{};
        float height{};
        float weaponCooldownTime{};

        float batchTargetX{};
        float batchCoordX{};
        float batchCoordY{};
        float batchMoveTimeleft{};
        float direction{};

        Dod::DBBuffer<int32_t> toRemove;

        float enemyWeaponCooldownTimeLeft{};
        Utils::RandomGenerator rand;

        [[nodiscard]] static Data load(Dod::MemPool& pool, int32_t& header) noexcept;

    };

}
