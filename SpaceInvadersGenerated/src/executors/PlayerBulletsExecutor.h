#pragma once

#include <Contexts/EnemyDimentionsContext.h>
#include <Contexts/ObstacleDimentionsContext.h>
#include <Contexts/PlayerBulletsContext.h>
#include <Contexts/RenderCommandsContext.h>
#include <Contexts/SceneContext.h>
#include <Contexts/SoundCmdsContext.h>
#include <Contexts/ToRemoveContext.h>
#include <Contexts/ToSpawnContext.h>
#include <Contexts/UnitsContext.h>

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{
    class PlayerBullets
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
        Context::Scene::Data sceneParametersContext;
        Context::PlayerBullets::Data parametersContext;
        Context::ToRemove::Data toRemoveContext;
        Context::ObstacleDimentions::Data obstacleDimentionsContext;
        Context::EnemyDimentions::Data enemyDimentionsContext;
        Context::RenderCommands::Data renderCmdsContext;
        Context::ToRemove::Data obstaclesToHitContext;
        Context::ToRemove::Data enemiesToHitContext;
        Context::SoundCmds::Data soundCmdsContext;
        const Dod::SharedContext::Controller<Context::ToSpawn::Data>* toSpawnContext{ nullptr };
        const Dod::SharedContext::Controller<Context::Units::Data>* obstaclesCoordsContext{ nullptr };
        const Dod::SharedContext::Controller<Context::Units::Data>* enemiesCoordsContext{ nullptr };
    };
}
