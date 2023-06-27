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

        void setSharedContext(const Dod::SharedContext::Controller<Context::ModelsToRender::Data>* sContext) { this->sModelsContext = sContext; };
        void setSharedContext(const Dod::SharedContext::Controller<Context::RenderCommands::Data>* sContext) { this->sRenderContext = sContext; };
        
        void flushSharedLocalContexts();
        [[nodiscard]] const auto& getApplicationInstanceContext() const { return this->applicationContext; }
    private:

    private:

        Game::Context::Common::Data commonContext;
        Game::Context::Application::Data applicationContext;
        Game::Context::RenderInternal::Data renderBufferContext;

        const Dod::SharedContext::Controller<Context::RenderCommands::Data>* sRenderContext{ nullptr };
        const Dod::SharedContext::Controller<Context::ModelsToRender::Data>* sModelsContext{ nullptr };

    };

};
