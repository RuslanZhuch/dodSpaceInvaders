
#include "game.h"
#include <chrono>

#include "MainExecution.h"
#include "SoundsExecutor.h"
#include "RenderExecutor.h"
#include <game/EnemiesExecution.h>
#include <game/ModelsExecutor.h>

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
    Dod::SharedContext::Controller<Game::Context::Sounds::Shared>& soundsContext,
    Dod::SharedContext::Controller<Game::Context::Models::Shared>& modelsContext,
    Dod::SharedContext::Controller<Game::Context::Render::Shared>& renderContext
)
{

    sounds.update(dt);
    enemies.update(dt);
    models.update(dt);
    const auto bContinue{ render.update(dt) };
    
    Dod::SharedContext::flush(&soundsContext);
    Dod::SharedContext::flush(&renderContext);
//    Dod::SharedContext::flush(&modelsContext);

    Dod::SharedContext::merge(&renderContext, enemies.getSharedLocalContext<Game::Context::Render::Shared>());
    Dod::SharedContext::merge(&modelsContext, models.getSharedLocalContext<Game::Context::Models::Shared>());
    exe.flushSharedLocalContexts();
    enemies.flushSharedLocalContexts();
    models.flushSharedLocalContexts();

    return bContinue;

}

void Game::run()
{

    Dod::SharedContext::Controller<Game::Context::Sounds::Shared> sContext;
    Dod::SharedContext::Controller<Game::Context::Models::Shared> sModels;
    Dod::SharedContext::Controller<Game::Context::Render::Shared> sRender;

    Game::ExecutionBlock::Main exe;
    exe.loadContext();
    exe.initiate();

    Game::ExecutionBlock::Models models;
    models.loadContext();
    models.initiate();

    Game::ExecutionBlock::Enemies enemies;
    enemies.loadContext();
    enemies.initiate();

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
            sContext,
            sModels,
            sRender
        ))
            break;

        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;
    }

}
