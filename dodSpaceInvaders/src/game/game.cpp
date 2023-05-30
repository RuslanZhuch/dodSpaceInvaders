
#include "game.h"
#include <chrono>

#include "MainExecution.h"
#include "SoundsExecutor.h"
#include "RenderExecutor.h"
#include <game/EnemiesExecution.h>
#include <game/ModelsExecutor.h>
#include <game/EnemiesBulletsExecutor.h>
#include <game/ObstaclesExecutor.h>

#include <dod/SharedContext.h>
#include <game/SoundsSContext.h>
#include <game/RenderContext.h>
#include <game/ModelsSharedContext.h>

bool msgLoop(
    float dt, 
    Game::ExecutionBlock::Main& exe, 
    Game::ExecutionBlock::Sounds& sounds, 
    Game::ExecutionBlock::Enemies& enemies, 
    Game::ExecutionBlock::Models& models, 
    Game::ExecutionBlock::Render& render, 
    Game::ExecutionBlock::EnemiesBullets& enemyBullets,
    Game::ExecutionBlock::Obstacles& obstacles,
    Dod::SharedContext::Controller<Game::Context::Sounds::Shared>& soundsContext,
    Dod::SharedContext::Controller<Game::Context::Models::Shared>& modelsContext,
    Dod::SharedContext::Controller<Game::Context::BulletsToSpawn::Shared>& bulletsToSpawnContext,
    Dod::SharedContext::Controller<Game::Context::Render::Shared>& renderContext
)
{

    sounds.update(dt);
    enemies.update(dt);
    models.update(dt);
    enemyBullets.update(dt);
    obstacles.update(dt);
    const auto bContinue{ render.update(dt) };
    
    Dod::SharedContext::flush(&soundsContext);
    Dod::SharedContext::flush(&renderContext);
    Dod::SharedContext::flush(&bulletsToSpawnContext);
//    Dod::SharedContext::flush(&modelsContext);

    Dod::SharedContext::merge(&renderContext, enemies.getSharedLocalContext<Game::Context::Render::Shared>());
    Dod::SharedContext::merge(&renderContext, enemyBullets.getSharedLocalContext<Game::Context::Render::Shared>());
    Dod::SharedContext::merge(&renderContext, obstacles.getSharedLocalContext<Game::Context::Render::Shared>());
    Dod::SharedContext::merge(&soundsContext, enemyBullets.getSharedLocalContext<Game::Context::Sounds::Shared>());
    Dod::SharedContext::merge(&modelsContext, models.getSharedLocalContext<Game::Context::Models::Shared>());
    Dod::SharedContext::merge(&bulletsToSpawnContext, enemies.getSharedLocalContext<Game::Context::BulletsToSpawn::Shared>());
    exe.flushSharedLocalContexts();
    enemies.flushSharedLocalContexts();
    models.flushSharedLocalContexts();
    enemyBullets.flushSharedLocalContexts();

    return bContinue;

}

void Game::run()
{

    Dod::SharedContext::Controller<Game::Context::Sounds::Shared> sContext;
    Dod::SharedContext::Controller<Game::Context::Models::Shared> sModels;
    Dod::SharedContext::Controller<Game::Context::Render::Shared> sRender;
    Dod::SharedContext::Controller<Game::Context::BulletsToSpawn::Shared> sBulletsToSpawn;

    Game::ExecutionBlock::Main exe;
    exe.loadContext();
    exe.initiate();

    Game::ExecutionBlock::Models models;
    models.loadContext();
    models.initiate();

    Game::ExecutionBlock::Enemies enemies;
    enemies.loadContext();
    enemies.initiate();

    Game::ExecutionBlock::EnemiesBullets enemyBullets;
    enemyBullets.loadContext();
    enemyBullets.setSharedContext(&sBulletsToSpawn);
    enemyBullets.initiate();

    Game::ExecutionBlock::Obstacles obstacles;
    obstacles.loadContext();
    obstacles.initiate();

    Game::ExecutionBlock::Render render;
    render.loadContext();
    render.setSharedContext(&sRender);
    render.setSharedContext(&sModels);
    render.initiate();

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

            sContext,
            sModels,
            sBulletsToSpawn,
            sRender
        ))
            break;

        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;
    }

}
