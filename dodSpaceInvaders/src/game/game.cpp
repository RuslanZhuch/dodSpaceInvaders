
#include "game.h"
#include <chrono>

//#include "MainExecution.h"
#include "SoundsExecutor.h"
#include "RenderExecutor.h"
#include <game/EnemiesExecution.h>
#include <game/ModelsExecutor.h>
#include <game/EnemiesBulletsExecutor.h>
#include <game/ObstaclesExecutor.h>
#include <game/PlayerExecutor.h>
#include <game/PlayerBulletsExecutor.h>
#include <game/SceneExecutor.h>

#include <dod/SharedContext.h>
#include <game/SoundsControl.h>
#include <game/RenderCommnads.h>
#include <game/ModelsToRender.h>
#include <game/Application.h>

#include <iostream>

void Game::run()
{

    Dod::SharedContext::Controller<Game::Context::SoundsController::Data> sContext;
    Dod::SharedContext::Controller<Game::Context::ModelsToRender::Data> sModels;
    Dod::SharedContext::Controller<Game::Context::RenderCommands::Data> renderCommands;
    Dod::SharedContext::Controller<Game::Context::BulletsToSpawn::Data> sEnBulletsToSpawn;
    Dod::SharedContext::Controller<Game::Context::BulletsToSpawn::Data> sPlayerBulletsToSpawn;
    Dod::SharedContext::Controller<Game::Context::ObjectsToHit::Data> sObstaclesToHit;
    Dod::SharedContext::Controller<Game::Context::ObstaclesCoordinates::Data> sObstacles;
    Dod::SharedContext::Controller<Game::Context::Units::Data> sEnemies;
    Dod::SharedContext::Controller<Game::Context::ObjectsToHit::Data> sEnemiesToHit;
    Dod::SharedContext::Controller<Game::Context::Units::Data> sPlayer;
    Dod::SharedContext::Controller<Game::Context::ObjectsToHit::Data> sPlayerToHit;
    Dod::SharedContext::Controller<Game::Context::Application::Data> sApplication;

    Game::ExecutionBlock::Models models;
    models.loadContext();
    models.initiate();

    Game::ExecutionBlock::Enemies enemies;
    enemies.loadContext();
    enemies.toHitSContext = &sEnemiesToHit;
    enemies.initiate();

    Game::ExecutionBlock::EnemiesBullets enemyBullets;
    enemyBullets.loadContext();
    enemyBullets.bulletsSContext = &sEnBulletsToSpawn;
    enemyBullets.obstaclesCoordinates = &sObstacles;
    enemyBullets.playerSContext = &sPlayer;
    enemyBullets.initiate();

    Game::ExecutionBlock::Obstacles obstacles;
    obstacles.loadContext();
    obstacles.obstaclesToHit = &sObstaclesToHit;
    obstacles.initiate();

    Game::ExecutionBlock::Render render;
    render.loadContext();
    render.sRenderContext = &renderCommands;
    render.sModelsContext = &sModels;
    render.initiate();

    Game::ExecutionBlock::PlayerBullets playerBullets;
    playerBullets.loadContext();
    playerBullets.bulletsSContext = &sPlayerBulletsToSpawn;
    playerBullets.obstaclesCoordinates = &sObstacles;
    playerBullets.enemiesSContext = &sEnemies;
    playerBullets.initiate();

    Game::ExecutionBlock::Player player;
    player.loadContext();
    player.toHitSContext = &sPlayerToHit;
    player.initiate();

    Game::ExecutionBlock::Sounds sounds;
    sounds.loadContext();
    sounds.sContext = &sContext;
    sounds.initiate();

    Game::ExecutionBlock::Scene scene;
    scene.loadContext();
    scene.initiate();

    float deltaTime{ 0.f };

    while (true)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };

//        std::cout << deltaTime << '\n';

        sounds.update(deltaTime);
        enemies.update(deltaTime);
        models.update(deltaTime);
        enemyBullets.update(deltaTime);
        obstacles.update(deltaTime);
        player.update(deltaTime);
        playerBullets.update(deltaTime);
        render.update(deltaTime);
        scene.update(deltaTime);

        Dod::SharedContext::flush(&sContext);
        Dod::SharedContext::flush(&renderCommands);
        Dod::SharedContext::flush(&sEnBulletsToSpawn);
        Dod::SharedContext::flush(&sPlayerBulletsToSpawn);
        Dod::SharedContext::flush(&sObstaclesToHit);
        Dod::SharedContext::flush(&sObstacles);
        Dod::SharedContext::flush(&sEnemies);
        Dod::SharedContext::flush(&sEnemiesToHit);
        Dod::SharedContext::flush(&sPlayer);
        Dod::SharedContext::flush(&sPlayerToHit);
        //    Dod::SharedContext::flush(&modelsContext);

        Dod::SharedContext::merge(&renderCommands, enemies.renderContext);
        Dod::SharedContext::merge(&renderCommands, enemyBullets.renderContext);
        Dod::SharedContext::merge(&renderCommands, playerBullets.renderContext);
        Dod::SharedContext::merge(&renderCommands, obstacles.renderContext);
        Dod::SharedContext::merge(&renderCommands, player.renderContext);
        Dod::SharedContext::merge(&renderCommands, scene.renderContext);
        Dod::SharedContext::merge(&sContext, enemyBullets.soundsContext);
        Dod::SharedContext::merge(&sContext, playerBullets.soundsContext);
        Dod::SharedContext::merge(&sModels, models.modelsContext);
        Dod::SharedContext::merge(&sEnBulletsToSpawn, enemies.bulletsToSpawnContext);
        Dod::SharedContext::merge(&sPlayerBulletsToSpawn, player.bulletsToSpawnContext);
        Dod::SharedContext::merge(&sObstaclesToHit, enemyBullets.obstaclesToHitContext);
        Dod::SharedContext::merge(&sObstaclesToHit, playerBullets.obstaclesToHitContext);
        Dod::SharedContext::merge(&sObstacles, obstacles.obstaclesCoordinates);
        Dod::SharedContext::merge(&sEnemies, enemies.unitsContext);
        Dod::SharedContext::merge(&sEnemiesToHit, playerBullets.enemiesToHitContext);
        Dod::SharedContext::merge(&sPlayer, player.unitContext);
        Dod::SharedContext::merge(&sPlayerToHit, enemyBullets.playerToHit);
        Dod::SharedContext::merge(&sApplication, render.applicationContext);

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
