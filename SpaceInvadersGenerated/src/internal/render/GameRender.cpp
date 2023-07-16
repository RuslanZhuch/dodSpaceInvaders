#include "GameRender.h"


void Game::SceneRenderer::drawObstacles(
    Game::GameRenderer& renderer, 
    Dod::ImBuffer<float> obstacleXCoords,
    Dod::ImBuffer<float> obstacleYCoords
)
{
    for (int32_t elId{ 0 }; elId < Dod::BufferUtils::getNumFilledElements(obstacleXCoords); ++elId)
    {
        const auto x{ Dod::BufferUtils::get(obstacleXCoords, elId) };
        const auto y{ Dod::BufferUtils::get(obstacleYCoords, elId) };
        Game::SceneRenderer::drawObstacle(renderer, x, y);
    }
}

void Game::SceneRenderer::drawBullets(
    Game::GameRenderer& renderer, 
    Dod::ImBuffer<float> xPositions, 
    Dod::ImBuffer<float> yPositions
)
{
    for (int32_t elId{ 0 }; elId < Dod::BufferUtils::getNumFilledElements(xPositions); ++elId)
        Game::SceneRenderer::drawBullet(renderer, Dod::BufferUtils::get(xPositions, elId), Dod::BufferUtils::get(yPositions, elId));
}

void Game::SceneRenderer::drawEnemies(
    Game::GameRenderer& renderer,
    Dod::ImBuffer<float> enemiesXCoords,
    Dod::ImBuffer<float> enemiesYCoords 
)
{
    for (int32_t elId{ 0 }; elId < Dod::BufferUtils::getNumFilledElements(enemiesXCoords); ++elId)
        Game::SceneRenderer::drawEnemy(renderer, Dod::BufferUtils::get(enemiesXCoords, elId), Dod::BufferUtils::get(enemiesYCoords, elId));
}