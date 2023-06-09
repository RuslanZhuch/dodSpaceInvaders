#pragma once

#include "SoundsControl.h"
#include "Sounds.h"

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

#include <memory>

namespace Game::ExecutionBlock
{

    class Sounds
    {

    public:
        void loadContext();

        void initiate();
        void update(float dt);

        Dod::MemPool memory;

        Context::Sounds::Data soundEngine;

        const Dod::SharedContext::Controller<Context::SoundsController::Data>* sContext{ nullptr };

    };

};
