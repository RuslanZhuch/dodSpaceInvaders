#pragma once

#include "EnemiesContext.h"
#include "PlayerContext.h"
#include "BulletsContext.h"
#include "ObstaclesContext.h"
#include "CommonContext.h"
#include "SceneContext.h"
#include "ObjectsToHitSContext.h"

#include "GameRender.h"
#include "SoundsSContext.h"
#include "RenderSharedContext.h"
#include "EnemiesContext.h"
#include "BulletsToSpawnSContext.h"
#include "UnitsSContext.h"

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{

    class Enemies
    {

    public:
        void loadContext();

        void initiate();
        void update(float dt);

        template<typename TContext>
        [[nodiscard]] const TContext& getSharedLocalContext();
        void flushSharedLocalContexts();

        void setSharedContext(const Dod::SharedContext::Controller<Context::ObjectsToHit::Shared>* sContext) { this->toHitSContext = sContext; };
    private:

    private:
        Dod::MemPool memory;

        Game::Context::Enemy::Data enemyContext;

        Context::Sounds::Shared soundsContext;
        Context::Render::Shared renderContext;
        Context::BulletsToSpawn::Shared bulletsToSpawnContext;
        Context::Units::Shared unitsContext;

        const Dod::SharedContext::Controller<Context::ObjectsToHit::Shared>* toHitSContext{ nullptr };
    };

};
