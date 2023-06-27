#pragma once

#include "PlayerContext.h"
#include "UnitsSContext.h"
#include "ObjectsToHit.h"

#include "GameRender.h"
#include "SoundsControl.h"
#include "RenderCommnads.h"
#include "BulletsToSpawn.h"

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{

    class Player
    {

    public:
        void loadContext();

        void initiate();
        void update(float dt);

        void flushSharedLocalContexts();

        Game::Context::Player::Data playerContext;

        Context::Units::Data unitContext;

        Context::SoundsController::Data soundsContext;
        Context::RenderCommands::Data renderContext;
        Context::BulletsToSpawn::Data bulletsToSpawnContext;

        const Dod::SharedContext::Controller<Context::ObjectsToHit::Data>* toHitSContext{ nullptr };

    };

};
