#pragma once

#include "EnemiesContext.h"
#include "PlayerContext.h"
#include "BulletsContext.h"
#include "ObstaclesContext.h"
#include "CommonContext.h"
#include "SceneContext.h"

#include "GameRender.h"
#include "ModelsSharedContext.h"

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
        [[nodiscard]] bool update(float dt);

        template<typename TContext>
        [[nodiscard]] const TContext& getSharedLocalContext();
        void flushSharedLocalContexts();
    private:

    private:
        Dod::MemPool memory;

        Context::Models::Shared modelsContext;

    };

};
