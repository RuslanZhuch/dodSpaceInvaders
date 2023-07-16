#pragma once

#include <Contexts/EnemiesContext.h>
#include <Contexts/RenderCommandsContext.h>
#include <Contexts/ToRemoveContext.h>
#include <Contexts/ToSpawnContext.h>
#include <Contexts/UnitsContext.h>

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{
    class Enemies
    {
    public:
        void loadContext() noexcept;
        void initiate() noexcept;
        void update(float dt) noexcept;
        void flushSharedLocalContexts() noexcept;

    private:
        void initImpl() noexcept;
        void updateImpl(float dt) noexcept;

    public:
        Dod::MemPool memory;
        Context::Enemies::Data parametersContext;
        Context::RenderCommands::Data renderCmdsContext;
        Context::Units::Data unitsContext;
        Context::ToSpawn::Data bulletsToSpawnContext;
        const Dod::SharedContext::Controller<Context::ToRemove::Data>* toHitContext{ nullptr };
    };
}
