
#include "game.h"
#include <chrono>

#include "MainExecution.h"

void msgLoop(float dt, Game::ExecutionBlock::Main& exe)
{
    exe.update(dt);
}

void Game::run()
{

    Game::ExecutionBlock::Main exe;

    exe.loadContext();
    exe.initiate();

    float deltaTime{ 0.f };

    while (exe.getWindow().isOpen())
    {
        const auto start{ std::chrono::high_resolution_clock::now() };
        msgLoop(deltaTime, exe);
        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;
    }

}
