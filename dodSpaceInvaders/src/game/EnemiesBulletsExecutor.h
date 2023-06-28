#pragma once

#include "EnemyBullets.h"
#include "Scene.h"
#include "UnitsSContext.h"

#include "GameRender.h"
#include "SoundsControl.h"
#include "RenderCommnads.h"
#include "EnemiesContext.h"
#include "ObjectsToHit.h"
#include "BulletsToSpawn.h"
#include "ObstacleParameter.h"
#include "ObstaclesCoordinates.h"
#include "Obstacles.h"
#include "PlayerDimentions.h"

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{

    class EnemiesBullets
    {

    public:
        void loadContext();

        void initiate();
        void update(float dt);

        void flushSharedLocalContexts();

        Game::Context::Scene::Data sceneParameters;
        Game::Context::EnemyBullets::Data enemyBulletsParameters;
//        Game::Context::Bullets::UnitsContext enemyBulletsContext;

        Context::SoundsController::Data soundsContext;
        Context::RenderCommands::Data renderContext;
        Context::ObjectsToHit::Data obstaclesToHitContext;
        Context::ObjectsToHit::Data bulletsToRemove;
        Context::ObjectsToHit::Data playerToHit;
        Context::ObstacleParameters::Data obstacleParameters;
        Context::PlayerDimentions::Data playerDimentions;

        const Dod::SharedContext::Controller<Context::BulletsToSpawn::Data>* bulletsSContext{ nullptr };
        const Dod::SharedContext::Controller<Context::Obstacles::Data>* obstaclesSContext{ nullptr };
        const Dod::SharedContext::Controller<Context::ObstaclesCoordinates::Data>* obstaclesCoordinates{ nullptr };
        const Dod::SharedContext::Controller<Context::Units::Data>* playerSContext{ nullptr };
    };

};
