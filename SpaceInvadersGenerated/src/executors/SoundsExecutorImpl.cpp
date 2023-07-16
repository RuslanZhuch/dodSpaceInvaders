#include "SoundsExecutor.h"

#include <dod/BufferUtils.h>
#include <iostream>


namespace Game::ExecutionBlock
{

    void Sounds::initImpl() noexcept
    {
        this->coreContext.soundsCore = std::make_unique<SoLoud::Soloud>();
        this->coreContext.soundsCore->init();

        Dod::BufferUtils::constructBack(this->coreContext.sounds);
        Dod::BufferUtils::constructBack(this->coreContext.sounds);
        Dod::BufferUtils::get(this->coreContext.sounds, 0).load("resources/sounds/weapons/shoot1.wav");
        Dod::BufferUtils::get(this->coreContext.sounds, 1).load("resources/sounds/weapons/shoot2.wav");
    }

    void Sounds::updateImpl(float dt) noexcept
    {

        const auto soundIdsToPlay{ Dod::SharedContext::get(this->cmdsContext).soundIdsToPlay };
        for (int32_t sfxId{}; sfxId < Dod::BufferUtils::getNumFilledElements(soundIdsToPlay); ++sfxId) {
            std::cout << "Play sound id " << Dod::BufferUtils::get(soundIdsToPlay, sfxId) << "\n";
            this->coreContext.soundsCore->play(Dod::BufferUtils::get(this->coreContext.sounds, Dod::BufferUtils::get(soundIdsToPlay, sfxId)));
        }

    }

}
