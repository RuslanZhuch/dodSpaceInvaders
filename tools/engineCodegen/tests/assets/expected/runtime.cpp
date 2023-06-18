#include <chrono>

void runtime()
{
    Dod::SharedContext::Controller<Game::Context::SContext1::Shared> sharedInst1Context;
    Dod::SharedContext::Controller<Game::Context::SContext1::Shared> sharedInst2Context;
    Dod::SharedContext::Controller<Game::Context::SContext2::Shared> sharedInst3Context;
    Dod::SharedContext::Controller<Game::Context::SContext2::Shared> sharedInst4Context;

    Game::ExecutionBlock::Executor1 executor1;
    executor1.loadContext();
    executor1.initiate();
    Game::ExecutionBlock::Executor2 executor2;
    executor2.loadContext();
    executor2.shared1Context = &sharedInst1Context;
    executor2.shared2Context = &sharedInst2Context;
    executor2.shared3Context = &sharedInst3Context;
    executor2.shared4Context = &sharedInst4Context;
    executor2.initiate();
    Game::ExecutionBlock::Executor3 executor3;
    executor3.loadContext();
    executor3.shared1_2Context = &sharedInst1Context;
    executor3.shared2_2Context = &sharedInst3Context;
    executor3.initiate();

    float deltaTime{};
    while(true)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };

        executor1.update(deltaTime);
        executor2.update(deltaTime);
        executor3.update(deltaTime);

        Dod::SharedContext::flush(&sharedInst1Context);
        Dod::SharedContext::flush(&sharedInst2Context);
        Dod::SharedContext::flush(&sharedInst4Context);

        executor1.flushSharedLocalContexts();
        executor2.flushSharedLocalContexts();
        executor3.flushSharedLocalContexts();

        Dod::SharedContext::merge(&sharedInst1Context, executor2.shared1Context);
        Dod::SharedContext::merge(&sharedInst1Context, executor3.shared1_2Context);
        Dod::SharedContext::merge(&sharedInst2Context, executor2.shared2Context);
        Dod::SharedContext::merge(&sharedInst3Context, executor2.shared3Context);
        Dod::SharedContext::merge(&sharedInst3Context, executor3.shared2_2Context);
        Dod::SharedContext::merge(&sharedInst4Context, executor2.shared4Context);

        for (int32_t cmdId{}; cmdId < Dod::BufferUtils::getNumFilledElements(sApplication.context.commands); ++cmdId)
        {
            if (Dod::BufferUtils::get(sApplication.context.commands, 0) == 1)
            {
                return;
            }
        }

        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) / 1'000'000'000.f;
    }
}
