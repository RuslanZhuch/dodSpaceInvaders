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

        template<typename TContext>
        [[nodiscard]] const TContext& getSharedLocalContext();
        void flushSharedLocalContexts();
    private:

    private:
        Dod::MemPool memory;

        Game::Context::Enemy::Parameters enemiesParameters;
        Game::Context::Enemy::BatchContext enemyBatchContext;
        Game::Context::Enemy::UnitsContext enemyUnitsContext;
        Game::Context::Enemy::WeaponContext enemyWeaponContext;

        Context::Sounds::Shared soundsContext;
        Context::Render::Shared renderContext;

    };

};
