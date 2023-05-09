#include "GameRender.h"


void Game::SceneRenderer::drawObstacles(
    Game::GameRenderer& renderer, 
    const Dod::ImBuffer<float>& obstacleXCoords,
    const Dod::ImBuffer<float>& obstacleYCoords
)
{
    for (int32_t elId{ 0 }; elId < obstacleXCoords.numOfFilledEls; ++elId)
    {
        const auto x{ Dod::BufferUtils::get(obstacleXCoords, elId) };
        const auto y{ Dod::BufferUtils::get(obstacleYCoords, elId) };
        Game::SceneRenderer::drawObstacle(renderer, x, y);
    }
}

void Game::SceneRenderer::drawBullets(
    Game::GameRenderer& renderer, 
    const Dod::ImBuffer<float>& xPositions, 
    const Dod::ImBuffer<float>& yPositions
)
{
    for (int32_t elId{ 0 }; elId < xPositions.numOfFilledEls; ++elId)
        Game::SceneRenderer::drawBullet(renderer, Dod::BufferUtils::get(xPositions, elId), Dod::BufferUtils::get(yPositions, elId));
}

void Game::SceneRenderer::drawEnemies(
    Game::GameRenderer& renderer,
    const Dod::ImBuffer<float>& enemiesXCoords,
    const Dod::ImBuffer<float>& enemiesYCoords 
)
{
    for (int32_t elId{ 0 }; elId < enemiesXCoords.numOfFilledEls; ++elId)
        Game::SceneRenderer::drawEnemy(renderer, Dod::BufferUtils::get(enemiesXCoords, elId), Dod::BufferUtils::get(enemiesYCoords, elId));
}