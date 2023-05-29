#pragma once

#include "EnemiesContext.h"
#include "PlayerContext.h"
#include "BulletsContext.h"
#include "ObstaclesContext.h"
#include "CommonContext.h"
#include "SceneContext.h"

#include "GameRender.h"
#include "ModelsSharedContext.h"
#include "RenderSharedContext.h"
#include "RenderContext.h"

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

#include <array>
#include <memory>

namespace Game::ExecutionBlock
{

    class Render
    {

    public:
        void loadContext();

        void initiate();
        [[nodiscard]] bool update(float dt);

        void setSharedContext(const Dod::SharedContext::Controller<Context::Models::Shared>* sContext) { this->sModelsContext = sContext; };
        void setSharedContext(const Dod::SharedContext::Controller<Context::Render::Shared>* sContext) { this->sRenderContext = sContext; };

    private:

    private:
        Dod::MemPool memory;
        Game::Context::Common::Parameters commonContext;

        Game::Context::Render::RenderBuffer renderBufferContext;

        std::unique_ptr<GameRenderer> gameRenderer;

        const Dod::SharedContext::Controller<Context::Render::Shared>* sRenderContext{ nullptr };
        const Dod::SharedContext::Controller<Context::Models::Shared>* sModelsContext{ nullptr };

    };

};
