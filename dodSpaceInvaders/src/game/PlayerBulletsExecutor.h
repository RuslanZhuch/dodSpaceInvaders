#pragma once

#include "EnemiesContext.h"
#include "PlayerContext.h"
#include "PlayerBullets.h"
#include "Scene.h"

#include "GameRender.h"
#include "SoundsControl.h"
#include "RenderCommnads.h"
#include "BulletsToSpawn.h"
#include "EnemiesContext.h"
#include "ObjectsToHit.h"
#include "Obstacles.h"
#include "ObstaclesCoordinates.h"
#include "ObstacleParameter.h"
#include "UnitsSContext.h"
#include "ObjectsToHit.h"

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{

    class PlayerBullets
    {

    public:
        void loadContext();

        void initiate();
        void update(float dt);

        void flushSharedLocalContexts();

        Game::Context::Scene::Data sceneParameters;
        Game::Context::PlayerBullets::Data playerBulletsContext;
//        Game::Context::Bullets::UnitsContext playerBulletsContext;

        Context::SoundsController::Data soundsContext;
        Context::RenderCommands::Data renderContext;
        Context::ObjectsToHit::Data obstaclesToHitContext;
        Context::ObjectsToHit::Data enemiesToHitContext;
        Context::ObstacleParameters::Data obstacleParameters;
        Context::ObjectsToHit::Data playerBulletsToRemove;

        const Dod::SharedContext::Controller<Context::BulletsToSpawn::Data>* bulletsSContext{ nullptr };
        const Dod::SharedContext::Controller<Context::Obstacles::Data>* obstaclesSContext{ nullptr };
        const Dod::SharedContext::Controller<Context::Units::Data>* enemiesSContext{ nullptr };
        const Dod::SharedContext::Controller<Context::ObstaclesCoordinates::Data>* obstaclesCoordinates{ nullptr };
    };

};
