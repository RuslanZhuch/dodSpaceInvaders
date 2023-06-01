#pragma once

#include "PlayerContext.h"
#include "BulletsContext.h"
#include "ObstaclesContext.h"
#include "CommonContext.h"
#include "SceneContext.h"

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
        [[nodiscard]] bool update(float dt);

        template<typename TContext>
        [[nodiscard]] const TContext& getSharedLocalContext();
        void flushSharedLocalContexts();
    private:

    private:
        Dod::MemPool memory;

        Game::Context::Player::Parameters playerParameters;
        Game::Context::Player::Position playerPositionContext;
        Game::Context::Player::Inputs playerInputsContext;
        Game::Context::Player::Movement playerMovementContext;
        Game::Context::Player::Fire playerFireContext;
        Game::Context::Player::Lifetime playerLifetimeContext;

        Context::Sounds::Shared soundsContext;
        Context::Render::Shared renderContext;
        Context::BulletsToSpawn::Shared bulletsToSpawnContext;

    };

};
