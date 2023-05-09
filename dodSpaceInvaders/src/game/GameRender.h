#pragma once

#include "SceneRenderer.h"

#include <dod/BufferUtils.h>

namespace Game::SceneRenderer
{

    void drawObstacles(
        Game::GameRenderer& renderer,
        const Dod::ImBuffer<float>& obstacleXCoords,
        const Dod::ImBuffer<float>& obstacleYCoords
    );

    void drawBullets(
        Game::GameRenderer& renderer,
        const Dod::ImBuffer<float>& xPositions,
        const Dod::ImBuffer<float>& yPositions
    );

    void drawEnemies(
        Game::GameRenderer& renderer,
        const Dod::ImBuffer<float>& enemiesXCoords,
        const Dod::ImBuffer<float>& enemiesYCoords
    );

};
