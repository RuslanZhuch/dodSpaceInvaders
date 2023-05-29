#pragma once

#include "PlayerContext.h"
#include "BulletsContext.h"
#include "ObstaclesContext.h"
#include "CommonContext.h"
#include "SceneContext.h"

#include "game/SoundsSContext.h"
#include "game/ModelsSharedContext.h"

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

#include <Soloud/include/soloud.h>
#include <Soloud/include/soloud_wav.h>

#include <array>
#include <memory>

namespace Game::ExecutionBlock
{

    class Main
    {

    public:
        void loadContext();

        void initiate();
        [[nodiscard]] bool update(float dt);

//        [[nodiscard]] auto& getWindow() { return this->gameRenderer->getWindow(); }

//        template<typename TContext>
//        [[nodiscard]] const TContext& getSharedLocalContext();
        void flushSharedLocalContexts();

    private:

    private:
        Dod::MemPool memory;

//        Game::Context::Scene::Parameters sceneParameters;

//        Game::Context::Player::Parameters playerParameters;
//        Game::Context::Player::Position playerPositionContext;
//        Game::Context::Player::Inputs playerInputsContext;
//        Game::Context::Player::Movement playerMovementContext;
//        Game::Context::Player::Fire playerFireContext;
//        Game::Context::Player::Lifetime playerLifetimeContext;

//        Game::Context::Bullets::Parameters playerBulletsParameters;
//        Game::Context::Bullets::Parameters enemyBulletsParameters;
//
//        Game::Context::Bullets::UnitsContext playerBulletsContext;
//        Game::Context::Bullets::UnitsContext enemyBulletsContext;
//
//        Game::Context::Obstacles::Parameters obstaclesParameters;
//        Game::Context::Obstacles::UnitsContext obstaclesContext;


//        Context::Sounds::Shared soundsContext;
    };

};
