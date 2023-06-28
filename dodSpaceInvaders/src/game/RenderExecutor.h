#pragma once

#include "Common.h"
#include "Application.h"

#include "ModelsToRender.h"
#include "RenderInternal.h"
#include "RenderCommnads.h"

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
        void update(float dt);

        void flushSharedLocalContexts();

        Game::Context::Common::Data commonContext;
        Game::Context::Application::Data applicationContext;
        Game::Context::RenderInternal::Data renderBufferContext;

        const Dod::SharedContext::Controller<Context::RenderCommands::Data>* sRenderContext{ nullptr };
        const Dod::SharedContext::Controller<Context::ModelsToRender::Data>* sModelsContext{ nullptr };

    };

};
