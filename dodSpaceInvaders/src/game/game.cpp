
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
#include <game/ApplicationSContext.h>

#include <iostream>

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
    Dod::SharedContext::Controller<Game::Context::Application::Shared> sApplication;

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

        std::cout << deltaTime << '\n';

        sounds.update(deltaTime);
        enemies.update(deltaTime);
        models.update(deltaTime);
        enemyBullets.update(deltaTime);
        obstacles.update(deltaTime);
        player.update(deltaTime);
        playerBullets.update(deltaTime);
        render.update(deltaTime);

        Dod::SharedContext::flush(&sContext);
        Dod::SharedContext::flush(&sRender);
        Dod::SharedContext::flush(&sEnBulletsToSpawn);
        Dod::SharedContext::flush(&sPlayerBulletsToSpawn);
        Dod::SharedContext::flush(&sObstaclesToHit);
        Dod::SharedContext::flush(&sObstacles);
        Dod::SharedContext::flush(&sEnemies);
        Dod::SharedContext::flush(&sEnemiesToHit);
        Dod::SharedContext::flush(&sPlayer);
        Dod::SharedContext::flush(&sPlayerToHit);
        //    Dod::SharedContext::flush(&modelsContext);

        Dod::SharedContext::merge(&sRender, enemies.getSharedLocalContext<Game::Context::Render::Shared>());
        Dod::SharedContext::merge(&sRender, enemyBullets.getSharedLocalContext<Game::Context::Render::Shared>());
        Dod::SharedContext::merge(&sRender, playerBullets.getSharedLocalContext<Game::Context::Render::Shared>());
        Dod::SharedContext::merge(&sRender, obstacles.getSharedLocalContext<Game::Context::Render::Shared>());
        Dod::SharedContext::merge(&sRender, player.getSharedLocalContext<Game::Context::Render::Shared>());
        Dod::SharedContext::merge(&sContext, enemyBullets.getSharedLocalContext<Game::Context::Sounds::Shared>());
        Dod::SharedContext::merge(&sContext, playerBullets.getSharedLocalContext<Game::Context::Sounds::Shared>());
        Dod::SharedContext::merge(&sModels, models.getSharedLocalContext<Game::Context::Models::Shared>());
        Dod::SharedContext::merge(&sEnBulletsToSpawn, enemies.getSharedLocalContext<Game::Context::BulletsToSpawn::Shared>());
        Dod::SharedContext::merge(&sPlayerBulletsToSpawn, player.getSharedLocalContext<Game::Context::BulletsToSpawn::Shared>());
        Dod::SharedContext::merge(&sObstaclesToHit, enemyBullets.getSharedLocalContext<Game::Context::ObjectsToHit::Shared>());
        Dod::SharedContext::merge(&sObstaclesToHit, playerBullets.getSharedLocalContext<Game::Context::ObjectsToHit::Shared>());
        Dod::SharedContext::merge(&sObstacles, obstacles.getSharedLocalContext<Game::Context::Obstacles::Shared>());
        Dod::SharedContext::merge(&sEnemies, enemies.getSharedLocalContext<Game::Context::Units::Shared>());
        Dod::SharedContext::merge(&sEnemiesToHit, playerBullets.getEnemiesToHitInstanceContext());
        Dod::SharedContext::merge(&sPlayer, player.getSharedLocalContext<Game::Context::Units::Shared>());
        Dod::SharedContext::merge(&sPlayerToHit, enemyBullets.getPlayerToHitInstanceContext());
        Dod::SharedContext::merge(&sApplication, render.getApplicationInstanceContext());

        exe.flushSharedLocalContexts();
        render.flushSharedLocalContexts();
        enemies.flushSharedLocalContexts();
        models.flushSharedLocalContexts();
        enemyBullets.flushSharedLocalContexts();
        playerBullets.flushSharedLocalContexts();
        obstacles.flushSharedLocalContexts();
        player.flushSharedLocalContexts();

        for (int32_t cmdId{}; cmdId < Dod::BufferUtils::getNumFilledElements(sApplication.context.commands); ++cmdId)
        {
            if (Dod::BufferUtils::get(sApplication.context.commands, 0) == 1)
                return;
        }

        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;
    }

}
