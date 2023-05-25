#pragma once

#include "EnemiesContext.h"
#include "PlayerContext.h"
#include "BulletsContext.h"
#include "ObstaclesContext.h"
#include "CommonContext.h"
#include "SceneContext.h"

#include "GameRender.h"
#include "SoundsSContext.h"
#include "EnemiesContext.h"

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{

    class Enemies
    {

    public:
        void loadContext();

        void initiate();
        [[nodiscard]] bool update(float dt);

        void setSharedContext(const Dod::SharedContext::Controller<Context::Sounds::Shared>* sContext) { this->soundsContext = sContext; };

    private:

    private:
        Dod::MemPool memory;

        Game::Context::Enemy::Parameters enemiesParameters;
        Game::Context::Enemy::BatchContext enemyBatchContext;
        Game::Context::Enemy::UnitsContext enemyUnitsContext;
        Game::Context::Enemy::WeaponContext enemyWeaponContext;

        const Dod::SharedContext::Controller<Context::Sounds::Shared>* soundsContext{ nullptr };

    };

};
