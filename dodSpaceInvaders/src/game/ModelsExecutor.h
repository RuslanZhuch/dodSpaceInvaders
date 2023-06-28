#pragma once

#include "GameRender.h"
#include "ModelsToRender.h"

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

#include <array>
#include <memory>

namespace Game::ExecutionBlock
{

    class Models
    {

    public:
        void loadContext();

        void initiate();
        void update(float dt);

        void flushSharedLocalContexts();
    private:
        void createEnemyModel();
        void createEnemyBulletModel();
        void createObstacleModel();
        void createPlayerModel();

    public:
        Context::ModelsToRender::Data modelsContext;

    };

};
