
#include "game.h"
#include <chrono>

#include "MainExecution.h"

bool msgLoop(float dt, Game::ExecutionBlock::Main& exe)
{
    return exe.update(dt);
}

void Game::run()
{

    Game::ExecutionBlock::Main exe;

    exe.loadContext();
    exe.initiate();

    float deltaTime{ 0.f };

    while (true)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };
        if (!msgLoop(deltaTime, exe))
            break;

        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;
    }

}
