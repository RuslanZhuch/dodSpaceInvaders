#pragma once

#include <dod/BufferUtils.h>

namespace Game::Gameplay::Bullets
{

    void collisionUpdate(
        Dod::DBBuffer<int32_t>& enemiesToRemove,
        Dod::DBBuffer<int32_t>& playerBulletsToRemove,
        const Dod::ImBuffer<float>& playersBulletXCoords,
        const Dod::ImBuffer<float>& playersBulletYCoords,
        const Dod::ImBuffer<float>& enemiesXCoords,
        const Dod::ImBuffer<float>& enemiesYCoords,
        float enemiesWidth,
        float enemiesHeight
    ) noexcept;

    void testWithObstacles(
        Dod::DBBuffer<int32_t>& bulletsToDelete,
        Dod::DBBuffer<int32_t>& obstaclesToHit,
        const Dod::ImBuffer<float>& bulletsX,
        const Dod::ImBuffer<float>& bulletsY,
        const Dod::ImBuffer<float>& obstaclesX,
        const Dod::ImBuffer<float>& obstaclesY,
        float obstaclesWidth,
        float obstaclesHeight
    ) noexcept;

};
