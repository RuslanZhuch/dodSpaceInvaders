
#include "game.h"
#include <chrono>

#include "MainExecution.h"
#include "SoundsExecutor.h"

#include <dod/SharedContext.h>
#include <game/SoundsSContext.h>

bool msgLoop(
    float dt, 
    Game::ExecutionBlock::Main& exe, 
    Game::ExecutionBlock::Sounds& sounds, 
    Dod::SharedContext::Controller<Game::Context::Sounds::Shared>& soundsContext
)
{

    const auto bContinue{ exe.update(dt) };
    sounds.update(dt);
    
    Dod::SharedContext::flush(&soundsContext);
    Dod::SharedContext::merge(&soundsContext, exe.getSharedLocalContext<Game::Context::Sounds::Shared>());
    exe.flushSharedLocalContexts();

    return bContinue;

}

void Game::run()
{

    Dod::SharedContext::Controller<Game::Context::Sounds::Shared> sContext;

    Game::ExecutionBlock::Main exe;

    exe.loadContext();
    exe.initiate();

    Game::ExecutionBlock::Sounds sounds;

    sounds.loadContext();
    sounds.setSharedContext(&sContext);
    sounds.initiate();

    float deltaTime{ 0.f };

    while (true)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };
        if (!msgLoop(deltaTime, exe, sounds, sContext))
            break;

        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;
    }

}
