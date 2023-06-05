
#include "game.h"
#include <chrono>

#include "MainExecution.h"
#include "SoundsExecutor.h"
#include "RenderExecutor.h"
#include <game/EnemiesExecution.h>
#include <game/ModelsExecutor.h>
#include <game/EnemiesBulletsExecutor.h>
#include <game/ObstaclesExecutor.h>
#include <game/PlayerExecutor.h>
#include <game/PlayerBulletsExecutor.h>

#include <dod/SharedContext.h>
#include <game/SoundsSContext.h>
#include <game/RenderContext.h>
#include <game/ModelsSharedContext.h>
#include <game/ObjectsToHitSContext.h>
#include <game/ObstaclesSContext.h>
#include <game/UnitsSContext.h>

bool msgLoop(
    float dt, 
    Game::ExecutionBlock::Main& exe, 
    Game::ExecutionBlock::Sounds& sounds, 
    Game::ExecutionBlock::Enemies& enemies, 
    Game::ExecutionBlock::Models& models, 
    Game::ExecutionBlock::Render& render, 
    Game::ExecutionBlock::EnemiesBullets& enemyBullets,
    Game::ExecutionBlock::Obstacles& obstacles,
    Game::ExecutionBlock::Player& player,
    Game::ExecutionBlock::PlayerBullets& playerBullets,

    Dod::SharedContext::Controller<Game::Context::Sounds::Shared>& soundsContext,
    Dod::SharedContext::Controller<Game::Context::Models::Shared>& modelsContext,
    Dod::SharedContext::Controller<Game::Context::BulletsToSpawn::Shared>& enemyBulletsToSpawnContext,
    Dod::SharedContext::Controller<Game::Context::BulletsToSpawn::Shared>& playerBulletsToSpawnContext,
    Dod::SharedContext::Controller<Game::Context::ObjectsToHit::Shared>& obstaclesToHitContext,
    Dod::SharedContext::Controller<Game::Context::Obstacles::Shared>& obstaclesContext,
    Dod::SharedContext::Controller<Game::Context::Units::Shared>& enemiesContext,
    Dod::SharedContext::Controller<Game::Context::ObjectsToHit::Shared>& enemiesToHitContext,
    Dod::SharedContext::Controller<Game::Context::Units::Shared>& playerContext,
    Dod::SharedContext::Controller<Game::Context::ObjectsToHit::Shared>& playerToHitContext,
    Dod::SharedContext::Controller<Game::Context::Render::Shared>& renderContext
)
{

    sounds.update(dt);
    enemies.update(dt);
    models.update(dt);
    enemyBullets.update(dt);
    obstacles.update(dt);
    player.update(dt);
    playerBullets.update(dt);
    const auto bContinue{ render.update(dt) };
    
    Dod::SharedContext::flush(&soundsContext);
    Dod::SharedContext::flush(&renderContext);
    Dod::SharedContext::flush(&enemyBulletsToSpawnContext);
    Dod::SharedContext::flush(&playerBulletsToSpawnContext);
    Dod::SharedContext::flush(&obstaclesToHitContext);
    Dod::SharedContext::flush(&obstaclesContext);
    Dod::SharedContext::flush(&enemiesContext);
    Dod::SharedContext::flush(&enemiesToHitContext);
    Dod::SharedContext::flush(&playerContext);
    Dod::SharedContext::flush(&playerToHitContext);
//    Dod::SharedContext::flush(&modelsContext);

    Dod::SharedContext::merge(&renderContext, enemies.getSharedLocalContext<Game::Context::Render::Shared>());
    Dod::SharedContext::merge(&renderContext, enemyBullets.getSharedLocalContext<Game::Context::Render::Shared>());
    Dod::SharedContext::merge(&renderContext, playerBullets.getSharedLocalContext<Game::Context::Render::Shared>());
    Dod::SharedContext::merge(&renderContext, obstacles.getSharedLocalContext<Game::Context::Render::Shared>());
    Dod::SharedContext::merge(&renderContext, player.getSharedLocalContext<Game::Context::Render::Shared>());
    Dod::SharedContext::merge(&soundsContext, enemyBullets.getSharedLocalContext<Game::Context::Sounds::Shared>());
    Dod::SharedContext::merge(&soundsContext, playerBullets.getSharedLocalContext<Game::Context::Sounds::Shared>());
    Dod::SharedContext::merge(&modelsContext, models.getSharedLocalContext<Game::Context::Models::Shared>());
    Dod::SharedContext::merge(&enemyBulletsToSpawnContext, enemies.getSharedLocalContext<Game::Context::BulletsToSpawn::Shared>());
    Dod::SharedContext::merge(&playerBulletsToSpawnContext, player.getSharedLocalContext<Game::Context::BulletsToSpawn::Shared>());
    Dod::SharedContext::merge(&obstaclesToHitContext, enemyBullets.getSharedLocalContext<Game::Context::ObjectsToHit::Shared>());
    Dod::SharedContext::merge(&obstaclesToHitContext, playerBullets.getSharedLocalContext<Game::Context::ObjectsToHit::Shared>());
    Dod::SharedContext::merge(&obstaclesContext, obstacles.getSharedLocalContext<Game::Context::Obstacles::Shared>());
    Dod::SharedContext::merge(&enemiesContext, enemies.getSharedLocalContext<Game::Context::Units::Shared>());
    Dod::SharedContext::merge(&enemiesToHitContext, playerBullets.getEnemiesToHitInstanceContext());
    Dod::SharedContext::merge(&playerContext, player.getSharedLocalContext<Game::Context::Units::Shared>());
    Dod::SharedContext::merge(&playerToHitContext, enemyBullets.getPlayerToHitInstanceContext());
    
    exe.flushSharedLocalContexts();
    enemies.flushSharedLocalContexts();
    models.flushSharedLocalContexts();
    enemyBullets.flushSharedLocalContexts();
    playerBullets.flushSharedLocalContexts();
    obstacles.flushSharedLocalContexts();
    player.flushSharedLocalContexts();

    return bContinue;

}

void Game::run()
{

    Dod::SharedContext::Controller<Game::Context::Sounds::Shared> sContext;
    Dod::SharedContext::Controller<Game::Context::Models::Shared> sModels;
    Dod::SharedContext::Controller<Game::Context::Render::Shared> sRender;
    Dod::SharedContext::Controller<Game::Context::BulletsToSpawn::Shared> sEnBulletsToSpawn;
    Dod::SharedContext::Controller<Game::Context::BulletsToSpawn::Shared> sPlayerBulletsToSpawn;
    Dod::SharedContext::Controller<Game::Context::ObjectsToHit::Shared> sObstaclesToHit;
    Dod::SharedContext::Controller<Game::Context::Obstacles::Shared> sObstacles;
    Dod::SharedContext::Controller<Game::Context::Units::Shared> sEnemies;
    Dod::SharedContext::Controller<Game::Context::ObjectsToHit::Shared> sEnemiesToHit;
    Dod::SharedContext::Controller<Game::Context::Units::Shared> sPlayer;
    Dod::SharedContext::Controller<Game::Context::ObjectsToHit::Shared> sPlayerToHit;

    Game::ExecutionBlock::Main exe;
    exe.loadContext();
    exe.initiate();

    Game::ExecutionBlock::Models models;
    models.loadContext();
    models.initiate();

    Game::ExecutionBlock::Enemies enemies;
    enemies.loadContext();
    enemies.setSharedContext(&sEnemiesToHit);
    enemies.initiate();

    Game::ExecutionBlock::EnemiesBullets enemyBullets;
    enemyBullets.loadContext();
    enemyBullets.setSharedContext(&sEnBulletsToSpawn);
    enemyBullets.setSharedContext(&sObstacles);
    enemyBullets.setSharedContext(&sPlayer);
    enemyBullets.initiate();

    Game::ExecutionBlock::Obstacles obstacles;
    obstacles.loadContext();
    obstacles.setSharedContext(&sObstaclesToHit);
    obstacles.initiate();

    Game::ExecutionBlock::Render render;
    render.loadContext();
    render.setSharedContext(&sRender);
    render.setSharedContext(&sModels);
    render.initiate();

    Game::ExecutionBlock::PlayerBullets playerBullets;
    playerBullets.loadContext();
    playerBullets.setSharedContext(&sPlayerBulletsToSpawn);
    playerBullets.setSharedContext(&sObstacles);
    playerBullets.setEnemiesSharedContext(&sEnemies);
    playerBullets.initiate();

    Game::ExecutionBlock::Player player;
    player.loadContext();
    player.setSharedContext(&sPlayerToHit);
    player.initiate();

    Game::ExecutionBlock::Sounds sounds;
    sounds.loadContext();
    sounds.setSharedContext(&sContext);
    sounds.initiate();

    float deltaTime{ 0.f };

    while (true)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };
        if (!msgLoop(
            deltaTime, 
            exe, 
            sounds, 
            enemies,
            models,
            render,
            enemyBullets,
            obstacles,
            player,
            playerBullets,

            sContext,
            sModels,
            sEnBulletsToSpawn,
            sPlayerBulletsToSpawn,
            sObstaclesToHit,
            sObstacles,
            sEnemies,
            sEnemiesToHit,
            sPlayer,
            sPlayerToHit,
            sRender
        ))
            break;

        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;
    }

}
