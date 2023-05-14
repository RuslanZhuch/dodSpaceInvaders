#pragma once

#include "EnemiesContext.h"
#include "PlayerContext.h"
#include "BulletsContext.h"
#include "ObstaclesContext.h"
#include "CommonContext.h"
#include "SceneContext.h"

#include "GameRender.h"

#include <dod/MemPool.h>

#include <memory>

namespace Game::ExecutionBlock
{

    class Main
    {

    public:
        void loadContext();

        void initiate();
        [[nodiscard]] bool update(float dt);

        [[nodiscard]] auto& getWindow() { return this->gameRenderer->getWindow(); }

    private:

    private:
        Dod::MemPool memory;

        Game::Context::Scene::Parameters sceneParameters;

        Game::Context::Enemy::Parameters enemiesParameters;
        Game::Context::Enemy::BatchContext enemyBatchContext;
        Game::Context::Enemy::UnitsContext enemyUnitsContext;
        Game::Context::Enemy::WeaponContext enemyWeaponContext;

        Game::Context::Player::Parameters playerParameters;
        Game::Context::Player::Position playerPositionContext;
        Game::Context::Player::Inputs playerInputsContext;
        Game::Context::Player::Movement playerMovementContext;
        Game::Context::Player::Fire playerFireContext;
        Game::Context::Player::Lifetime playerLifetimeContext;

        Game::Context::Bullets::Parameters playerBulletsParameters;
        Game::Context::Bullets::Parameters enemyBulletsParameters;

        Game::Context::Bullets::UnitsContext playerBulletsContext;
        Game::Context::Bullets::UnitsContext enemyBulletsContext;

        Game::Context::Obstacles::Parameters obstaclesParameters;
        Game::Context::Obstacles::UnitsContext obstaclesContext;

        Game::Context::Common::Parameters commonContext;

        std::unique_ptr<GameRenderer> gameRenderer;

    };

};
