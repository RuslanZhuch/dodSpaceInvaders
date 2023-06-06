#pragma once

#include "EnemiesContext.h"
#include "PlayerContext.h"
#include "BulletsContext.h"
#include "ObstaclesContext.h"
#include "CommonContext.h"
#include "SceneContext.h"

#include "GameRender.h"
#include "SoundsSContext.h"

#include <dod/MemPool.h>
#include <dod/SharedContext.h>

#include <Soloud/include/soloud.h>
#include <Soloud/include/soloud_wav.h>

#include <array>
#include <memory>

namespace Game::ExecutionBlock
{

    class Sounds
    {

    public:
        void loadContext();

        void initiate();
        void update(float dt);

        void setSharedContext(const Dod::SharedContext::Controller<Context::Sounds::Shared>* sContext) { this->sContext = sContext; };

    private:

    private:
        Dod::MemPool memory;

        std::unique_ptr<SoLoud::Soloud> soundsCore{};
        Dod::DBBuffer<SoLoud::Wav> sounds;

        const Dod::SharedContext::Controller<Context::Sounds::Shared>* sContext{ nullptr };

    };

};
