#pragma once

#include "PlayerContext.h"
#include "BulletsContext.h"
#include "ObstaclesContext.h"
#include "CommonContext.h"
#include "SceneContext.h"
#include "UnitsSContext.h"
#include "ObjectsToHitSContext.h"

#include "GameRender.h"
#include "SoundsSContext.h"
#include "RenderSharedContext.h"
#include "BulletsToSpawnSContext.h"

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

        template<typename TContext>
        [[nodiscard]] const TContext& getSharedLocalContext();
        void flushSharedLocalContexts();

        void setSharedContext(const Dod::SharedContext::Controller<Context::ObjectsToHit::Shared>* sContext) { this->toHitSContext = sContext; };
    private:

    private:
        Dod::MemPool memory;

        Game::Context::Player::Parameters playerParameters;
        Game::Context::Player::Position playerPositionParameters;
        Game::Context::Player::Inputs playerInputsContext;
        Game::Context::Player::Movement playerMovementContext;
        Game::Context::Player::Fire playerFireContext;
        Game::Context::Player::Lifetime playerLifetimeContext;

        Context::Units::Shared unitContext;

        Context::Sounds::Shared soundsContext;
        Context::Render::Shared renderContext;
        Context::BulletsToSpawn::Shared bulletsToSpawnContext;

        const Dod::SharedContext::Controller<Context::ObjectsToHit::Shared>* toHitSContext{ nullptr };

    };

};
