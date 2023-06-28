#pragma once

#include "Obstacles.h"
#include "ObstacleParameter.h"

#include "GameRender.h"
#include "SoundsControl.h"
#include "RenderCommnads.h"
#include "EnemiesContext.h"
#include "ObjectsToHit.h"
#include "UnitsSContext.h"
#include "ObstaclesCoordinates.h"

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

namespace Game::ExecutionBlock
{

    class Obstacles
    {

    public:
        void loadContext();

        void initiate();
        void update(float dt);

        void flushSharedLocalContexts();

        Dod::MemPool memory;

        Game::Context::Obstacles::Data obstaclesParameters;
        Game::Context::ObstacleParameters::Data obstaclesDimentionParameters;
        Game::Context::ObstaclesCoordinates::Data obstaclesCoordinates;
        Game::Context::ObjectsToHit::Data obstaclesToRemove;

        Context::SoundsController::Data soundsContext;
        Context::RenderCommands::Data renderContext;
//        Context::Obstacles::Shared obstaclesSContext;

        const Dod::SharedContext::Controller<Context::ObjectsToHit::Data>* obstaclesToHit{ nullptr };
    };

};
