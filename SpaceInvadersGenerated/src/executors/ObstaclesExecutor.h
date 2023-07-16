#pragma once

#include <Contexts/ObstacleDimentionsContext.h>
#include <Contexts/ObstaclesContext.h>
#include <Contexts/RenderCommandsContext.h>
#include <Contexts/ToRemoveContext.h>
#include <Contexts/UnitsContext.h>

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{
    class Obstacles
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
        Context::Units::Data obstaclesCoordsContext;
        Context::Obstacles::Data parametersContext;
        Context::ObstacleDimentions::Data dimentionsContext;
        Context::ToRemove::Data toRemoveContext;
        Context::RenderCommands::Data renderCmdsContext;
        const Dod::SharedContext::Controller<Context::ToRemove::Data>* toHitContext{ nullptr };
    };
}
