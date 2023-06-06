#pragma once

#include "EnemiesContext.h"
#include "PlayerContext.h"
#include "BulletsContext.h"
#include "ObstaclesContext.h"
#include "CommonContext.h"
#include "SceneContext.h"
#include "ObjectsToHitSContext.h"
#include "ObstaclesSContext.h"

#include "GameRender.h"
#include "SoundsSContext.h"
#include "RenderSharedContext.h"
#include "EnemiesContext.h"
#include "BulletsToSpawnSContext.h"

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{

    class Obstacles
    {

    public:
        void loadContext();

        void initiate();
        void update(float dt);

        template<typename TContext>
        [[nodiscard]] const TContext& getSharedLocalContext();
        void flushSharedLocalContexts();

        void setSharedContext(const Dod::SharedContext::Controller<Context::ObjectsToHit::Shared>* sContext) { this->sContext = sContext; };

    private:

    private:
        Dod::MemPool memory;

        Game::Context::Obstacles::Parameters obstaclesParameters;
        Game::Context::Obstacles::UnitsContext obstaclesContext;

        Context::Sounds::Shared soundsContext;
        Context::Render::Shared renderContext;
        Context::Obstacles::Shared obstaclesSContext;

        const Dod::SharedContext::Controller<Context::ObjectsToHit::Shared>* sContext{ nullptr };
    };

};
