#pragma once

#include "../utils/randomGenerator.h"
#include <dod/BufferUtils.h>

namespace Game::Gameplay::Enemies
{

    auto enemiesBatchUpdate(
        float batchDirection, 
        float batchTargetX, 
        float batchTargetY, 
        bool strobe
    );

    void enemiesLifetimeUpdate(
        Dod::ImBuffer<int32_t> enemiesToRemove,
        Dod::DBBuffer<float>& enemiesX,
        Dod::DBBuffer<float>& enemiesY
    );

    void enemiesUpdate(
        float dt,
        float& batchMoveTimeleft,
        float& moveDirection,
        float& batchTargetX,
        float& batchCoordX,
        float& batchCoordY,
        Dod::DBBuffer<float>& enemiesXCoords,
        Dod::DBBuffer<float>& enemiesYCoords
    );

    int32_t updateEnemyBulletsCreation(
        float dt,
        float& enemyWeaponCooldownTimeLeft,
        int32_t numOfEnemiesAlive
    ) noexcept;

    void createBulletsSFx(
        Dod::DBBuffer<int32_t>& soundIds,
        int32_t numOfBulletsToCreate
    ) noexcept;

    void generateEnemyBullets(
        int32_t numOfBulletsToCreate,

        Dod::DBBuffer<float>& bulletsXCoords,
        Dod::DBBuffer<float>& bulletsYCoords,

        Game::Utils::RandomGenerator& rand,

        const Dod::ImBuffer<float> enemiesXCoords,
        const Dod::ImBuffer<float> enemiesYCoords
    );

    void generateEnemies(
        const int32_t numOfEnemiesPerRow,
        const int32_t numOfEnemiesCols,
        const float enemiesXStride,
        const float enemiesYStride,
        float& batchXCoord,
        float& batchYCoord,
        Dod::DBBuffer<float>& xCoords,
        Dod::DBBuffer<float>& yCoords
    );

}
