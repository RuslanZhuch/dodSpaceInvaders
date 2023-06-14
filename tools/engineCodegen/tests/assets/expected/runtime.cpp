#include <chrono>

void runtime()
{
    Dod::SharedContext::Controller<Game::Context::SContext1::Shared> shared1Context;
    Dod::SharedContext::Controller<Game::Context::SContext1::Shared> shared2Context;
    Dod::SharedContext::Controller<Game::Context::SContext2::Shared> shared3Context;

    Game::ExecutionBlock::Executor1 executor1;
    executor1.loadContext();
    executor1.initiate();
    Game::ExecutionBlock::Executor2 executor2;
    executor2.loadContext();
    executor2.initiate();

    float deltaTime{};
    while(true)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };

        executor1.update(deltaTime);
        executor2.update(deltaTime);

        executor1.flushSharedLocalContexts();
        executor2.flushSharedLocalContexts();

        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;
    }
}
