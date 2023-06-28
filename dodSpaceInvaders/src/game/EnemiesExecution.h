#pragma once

#include "EnemiesContext.h"
#include "PlayerContext.h"
#include "ObjectsToHit.h"

#include "GameRender.h"
#include "SoundsControl.h"
#include "RenderCommnads.h"
#include "EnemiesContext.h"
#include "BulletsToSpawn.h"
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

        void flushSharedLocalContexts();

        Dod::MemPool memory;

        Game::Context::Enemy::Data enemyContext;

        Context::SoundsController::Data soundsContext;
        Context::RenderCommands::Data renderContext;
        Context::BulletsToSpawn::Data bulletsToSpawnContext;
        Context::Units::Data unitsContext;

        const Dod::SharedContext::Controller<Context::ObjectsToHit::Data>* toHitSContext{ nullptr };
    };

};
