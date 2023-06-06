#pragma once

#include "EnemiesContext.h"
#include "PlayerContext.h"
#include "BulletsContext.h"
#include "ObstaclesContext.h"
#include "CommonContext.h"
#include "SceneContext.h"
#include "UnitsSContext.h"

#include "GameRender.h"
#include "SoundsSContext.h"
#include "RenderSharedContext.h"
#include "BulletsToSpawnSContext.h"
#include "EnemiesContext.h"
#include "ObjectsToHitSContext.h"
#include "ObstaclesSContext.h"

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{

    class EnemiesBullets
    {

    public:
        void loadContext();

        void initiate();
        void update(float dt);

        template<typename TContext>
        [[nodiscard]] const TContext& getSharedLocalContext();

        [[nodiscard]] const auto& getPlayerToHitInstanceContext() { return this->playerToHit; };
        void flushSharedLocalContexts();

        void setSharedContext(const Dod::SharedContext::Controller<Context::BulletsToSpawn::Shared>* sContext) { this->bulletsSContext = sContext; };
        void setSharedContext(const Dod::SharedContext::Controller<Context::Obstacles::Shared>* sContext) { this->obstaclesSContext = sContext; };
        void setSharedContext(const Dod::SharedContext::Controller<Context::Units::Shared>* sContext) { this->playerSContext = sContext; };

    private:

    private:
        Dod::MemPool memory;

        Game::Context::Scene::Parameters sceneParameters;
        Game::Context::Bullets::Parameters enemyBulletsParameters;
        Game::Context::Bullets::UnitsContext enemyBulletsContext;

        Context::Sounds::Shared soundsContext;
        Context::Render::Shared renderContext;
        Context::ObjectsToHit::Shared obstaclesToHitContext;
        Context::ObjectsToHit::Shared playerToHit;

        const Dod::SharedContext::Controller<Context::BulletsToSpawn::Shared>* bulletsSContext{ nullptr };
        const Dod::SharedContext::Controller<Context::Obstacles::Shared>* obstaclesSContext{ nullptr };
        const Dod::SharedContext::Controller<Context::Units::Shared>* playerSContext{ nullptr };
    };

};
