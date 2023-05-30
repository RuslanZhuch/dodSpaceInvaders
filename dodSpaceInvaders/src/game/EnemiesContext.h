#pragma once

#include <dod/Buffers.h>

#include <utils/randomGenerator.h>

namespace Game::Context::Enemy
{

    struct Parameters
    {
        int32_t numOfEnemiesPerRow{};
        int32_t numOfEnemiesCols{};
        float enemiesXStride{};
        float enemiesYStride{};
        float width{};
        float height{};
        float weaponCooldownTime{};
    };

    struct BatchContext
    {
        float batchTargetX{};
        float batchCoordX{};
        float batchCoordY{};
        float batchMoveTimeleft{};
        float direction = 1.f;
    };

    struct UnitsContext
    {
//        Dod::DBBuffer<float> xCoords;
//        Dod::DBBuffer<float> yCoords;
        Dod::DBBuffer<int32_t> toRemove;
    };

    struct WeaponContext
    {
        float enemyWeaponCooldownTimeLeft{};
        Utils::RandomGenerator rand;
    };

//    void initiate(
//        Parameters& parameters,
//        BatchContext& batchContext,
//        UnitsContext& unitsContext,
//        BulletsContext& bulletsContext,
//        WeaponContext& weaponContext,
//        Dod::MemPool& memory,
//        int32_t& header
//    );

}
