#pragma once

#include "SceneRenderer.h"

#include <dod/BufferUtils.h>

namespace Game::SceneRenderer
{

    void drawObstacles(
        Game::GameRenderer& renderer,
        Dod::ImBuffer<float> obstacleXCoords,
        Dod::ImBuffer<float> obstacleYCoords
    );

    void drawBullets(
        Game::GameRenderer& renderer,
        Dod::ImBuffer<float> xPositions,
        Dod::ImBuffer<float> yPositions
    );

    void drawEnemies(
        Game::GameRenderer& renderer,
        Dod::ImBuffer<float> enemiesXCoords,
        Dod::ImBuffer<float> enemiesYCoords
    );

};
