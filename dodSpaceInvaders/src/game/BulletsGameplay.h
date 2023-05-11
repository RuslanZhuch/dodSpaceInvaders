#pragma once

#include <dod/BufferUtils.h>

namespace Game::Gameplay::Bullets
{

    void collisionUpdate(
        Dod::DBBuffer<int32_t>& enemiesToRemove,
        Dod::DBBuffer<int32_t>& playerBulletsToRemove,
        Dod::ImBuffer<float> playersBulletXCoords,
        Dod::ImBuffer<float> playersBulletYCoords,
        Dod::ImBuffer<float> enemiesXCoords,
        Dod::ImBuffer<float> enemiesYCoords,
        float enemiesWidth,
        float enemiesHeight
    ) noexcept;

    void testWithObstacles(
        Dod::DBBuffer<int32_t>& bulletsToDelete,
        Dod::DBBuffer<int32_t>& obstaclesToHit,
        Dod::ImBuffer<float> bulletsX,
        Dod::ImBuffer<float> bulletsY,
        Dod::ImBuffer<float> obstaclesX,
        Dod::ImBuffer<float> obstaclesY,
        float obstaclesWidth,
        float obstaclesHeight
    ) noexcept;

};
