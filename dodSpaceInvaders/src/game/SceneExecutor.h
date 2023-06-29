#pragma once

#include "RenderCommnads.h"

#include <dod/MemPool.h>

#include <memory>

namespace Game::ExecutionBlock
{

    class Scene
    {

    public:
        void loadContext();

        void initiate();
        void update(float dt);

        Context::RenderCommands::Data renderContext;

    };

};
