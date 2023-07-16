#include <Contexts/ApplicationContext.h>
#include <Contexts/ModelsToRenderContext.h>
#include <Contexts/RenderCommandsContext.h>
#include <Contexts/SoundCmdsContext.h>
#include <Contexts/ToRemoveContext.h>
#include <Contexts/ToSpawnContext.h>
#include <Contexts/UnitsContext.h>

#include <executors/RenderExecutor.h>
#include <executors/ModelsExecutor.h>
#include <executors/SceneExecutor.h>
#include <executors/PlayerExecutor.h>
#include <executors/PlayerBulletsExecutor.h>
#include <executors/ObstaclesExecutor.h>
#include <executors/EnemiesExecutor.h>
#include <executors/EnemyBulletsExecutor.h>
#include <executors/SoundsExecutor.h>

#include <dod/SharedContext.h>
#include <dod/BufferUtils.h>
#include <chrono>

int main()
{
    Dod::SharedContext::Controller<Game::Context::Application::Data> sApplicationContext;
    Dod::SharedContext::Controller<Game::Context::ModelsToRender::Data> modelsToRenderContext;
    Dod::SharedContext::Controller<Game::Context::RenderCommands::Data> renderCmdsContext;
    Dod::SharedContext::Controller<Game::Context::ToSpawn::Data> playerBulletsToSpawnContext;
    Dod::SharedContext::Controller<Game::Context::Units::Data> playerCoordContext;
    Dod::SharedContext::Controller<Game::Context::ToRemove::Data> playerToHitContext;
    Dod::SharedContext::Controller<Game::Context::ToRemove::Data> obstaclesToHitContext;
    Dod::SharedContext::Controller<Game::Context::Units::Data> obstaclesCoordsContext;
    Dod::SharedContext::Controller<Game::Context::ToRemove::Data> enemiesToHitContext;
    Dod::SharedContext::Controller<Game::Context::Units::Data> enemiesCoordsContext;
    Dod::SharedContext::Controller<Game::Context::ToSpawn::Data> enemiesBulletsToSpawnContext;
    Dod::SharedContext::Controller<Game::Context::SoundCmds::Data> soundCmdsContext;

    Game::ExecutionBlock::Render render;
    render.loadContext();
    render.modelsIdsContext = &modelsToRenderContext;
    render.renderCmdsContext = &renderCmdsContext;
    render.initiate();
    Game::ExecutionBlock::Models models;
    models.loadContext();
    models.initiate();
    Game::ExecutionBlock::Scene scene;
    scene.loadContext();
    scene.initiate();
    Game::ExecutionBlock::Player player;
    player.loadContext();
    player.toHitContext = &playerToHitContext;
    player.initiate();
    Game::ExecutionBlock::PlayerBullets playerBullets;
    playerBullets.loadContext();
    playerBullets.toSpawnContext = &playerBulletsToSpawnContext;
    playerBullets.obstaclesCoordsContext = &obstaclesCoordsContext;
    playerBullets.enemiesCoordsContext = &enemiesCoordsContext;
    playerBullets.initiate();
    Game::ExecutionBlock::Obstacles obstacles;
    obstacles.loadContext();
    obstacles.toHitContext = &obstaclesToHitContext;
    obstacles.initiate();
    Game::ExecutionBlock::Enemies enemies;
    enemies.loadContext();
    enemies.toHitContext = &enemiesToHitContext;
    enemies.initiate();
    Game::ExecutionBlock::EnemyBullets enemyBullets;
    enemyBullets.loadContext();
    enemyBullets.toSpawnContext = &enemiesBulletsToSpawnContext;
    enemyBullets.obstaclesCoordsContext = &obstaclesCoordsContext;
    enemyBullets.playerCoordContext = &playerCoordContext;
    enemyBullets.initiate();
    Game::ExecutionBlock::Sounds sounds;
    sounds.loadContext();
    sounds.cmdsContext = &soundCmdsContext;
    sounds.initiate();

    float deltaTime{};
    while(true)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };

        render.update(deltaTime);
        models.update(deltaTime);
        scene.update(deltaTime);
        player.update(deltaTime);
        playerBullets.update(deltaTime);
        obstacles.update(deltaTime);
        enemies.update(deltaTime);
        enemyBullets.update(deltaTime);
        sounds.update(deltaTime);

        Dod::SharedContext::flush(&renderCmdsContext);
        Dod::SharedContext::flush(&playerBulletsToSpawnContext);
        Dod::SharedContext::flush(&obstaclesToHitContext);
        Dod::SharedContext::flush(&obstaclesCoordsContext);
        Dod::SharedContext::flush(&enemiesToHitContext);
        Dod::SharedContext::flush(&enemiesCoordsContext);
        Dod::SharedContext::flush(&enemiesBulletsToSpawnContext);
        Dod::SharedContext::flush(&playerToHitContext);
        Dod::SharedContext::flush(&playerCoordContext);
        Dod::SharedContext::flush(&soundCmdsContext);

        Dod::SharedContext::merge(&sApplicationContext, render.applicationContext);
        Dod::SharedContext::merge(&renderCmdsContext, scene.renderCmdsContext);
        Dod::SharedContext::merge(&renderCmdsContext, player.renderCmdsContext);
        Dod::SharedContext::merge(&renderCmdsContext, obstacles.renderCmdsContext);
        Dod::SharedContext::merge(&renderCmdsContext, enemies.renderCmdsContext);
        Dod::SharedContext::merge(&renderCmdsContext, playerBullets.renderCmdsContext);
        Dod::SharedContext::merge(&renderCmdsContext, enemyBullets.renderCmdsContext);
        Dod::SharedContext::merge(&enemiesCoordsContext, enemies.unitsContext);
        Dod::SharedContext::merge(&enemiesBulletsToSpawnContext, enemies.bulletsToSpawnContext);
        Dod::SharedContext::merge(&obstaclesCoordsContext, obstacles.obstaclesCoordsContext);
        Dod::SharedContext::merge(&playerBulletsToSpawnContext, player.bulletsToSpawnContext);
        Dod::SharedContext::merge(&playerCoordContext, player.unitContext);
        Dod::SharedContext::merge(&obstaclesToHitContext, playerBullets.obstaclesToHitContext);
        Dod::SharedContext::merge(&obstaclesToHitContext, enemyBullets.obstaclesToHitContext);
        Dod::SharedContext::merge(&enemiesToHitContext, playerBullets.enemiesToHitContext);
        Dod::SharedContext::merge(&soundCmdsContext, playerBullets.soundCmdsContext);
        Dod::SharedContext::merge(&soundCmdsContext, enemyBullets.soundCmdsContext);
        Dod::SharedContext::merge(&modelsToRenderContext, models.modelsContext);
        Dod::SharedContext::merge(&playerToHitContext, enemyBullets.playerToHitContext);

        render.flushSharedLocalContexts();
        models.flushSharedLocalContexts();
        scene.flushSharedLocalContexts();
        player.flushSharedLocalContexts();
        playerBullets.flushSharedLocalContexts();
        obstacles.flushSharedLocalContexts();
        enemies.flushSharedLocalContexts();
        enemyBullets.flushSharedLocalContexts();
        sounds.flushSharedLocalContexts();

        for (int32_t cmdId{}; cmdId < Dod::BufferUtils::getNumFilledElements(sApplicationContext.context.commands); ++cmdId)
        {
            if (Dod::BufferUtils::get(sApplicationContext.context.commands, 0) == 1)
            {
                return 0;
            }
        }

        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;
    }
}
