#pragma once

#include "EnemiesContext.h"
#include "PlayerContext.h"
#include "BulletsContext.h"
#include "ObstaclesContext.h"
#include "CommonContext.h"
#include "SceneContext.h"

#include "GameRender.h"
#include "SoundsSContext.h"
#include "RenderSharedContext.h"
#include "BulletsToSpawnSContext.h"
#include "EnemiesContext.h"
#include "ObjectsToHitSContext.h"
#include "ObstaclesSContext.h"
#include "UnitsSContext.h"

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{

    class PlayerBullets
    {

    public:
        void loadContext();

        void initiate();
        [[nodiscard]] bool update(float dt);

        template<typename TContext>
        [[nodiscard]] const TContext& getSharedLocalContext();

        [[nodiscard]] const auto& getEnemiesToHitInstanceContext() { return this->enemiesToHitContext; }

        void flushSharedLocalContexts();

        void setSharedContext(const Dod::SharedContext::Controller<Context::BulletsToSpawn::Shared>* sContext) { this->bulletsSContext = sContext; };
        void setSharedContext(const Dod::SharedContext::Controller<Context::Obstacles::Shared>* sContext) { this->obstaclesSContext = sContext; };
        void setEnemiesSharedContext(const Dod::SharedContext::Controller<Context::Units::Shared>* sContext) { this->enemiesSContext = sContext; };

    private:

    private:
        Dod::MemPool memory;

        Game::Context::Scene::Parameters sceneParameters;
        Game::Context::Bullets::Parameters playerBulletsParameters;
        Game::Context::Bullets::UnitsContext playerBulletsContext;

        Context::Sounds::Shared soundsContext;
        Context::Render::Shared renderContext;
        Context::ObjectsToHit::Shared obstaclesToHitContext;
        Context::ObjectsToHit::Shared enemiesToHitContext;

        const Dod::SharedContext::Controller<Context::BulletsToSpawn::Shared>* bulletsSContext{ nullptr };
        const Dod::SharedContext::Controller<Context::Obstacles::Shared>* obstaclesSContext{ nullptr };
        const Dod::SharedContext::Controller<Context::Units::Shared>* enemiesSContext{ nullptr };
    };

};
