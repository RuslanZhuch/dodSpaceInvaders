#include "SoundsExecutor.h"

#include <dod/BufferUtils.h>

#include <Soloud/include/soloud.h>
#include <Soloud/include/soloud_wav.h>

void Game::ExecutionBlock::Sounds::loadContext()
{
    this->soundEngine.load();
}

void Game::ExecutionBlock::Sounds::initiate()
{
    this->soundEngine.soundsCore = std::make_unique<SoLoud::Soloud>();
    this->soundEngine.soundsCore->init();

    this->memory.allocate(sizeof(SoLoud::Wav) * 16);
//    int32_t header{};
//    Dod::BufferUtils::initFromMemory(this->sounds, Dod::MemUtils::stackAquire(this->memory, sizeof(SoLoud::Wav) * 16, header));

    Dod::BufferUtils::constructBack(this->soundEngine.sounds);
    Dod::BufferUtils::constructBack(this->soundEngine.sounds);
    Dod::BufferUtils::get(this->soundEngine.sounds, 0).load("resources/sounds/weapons/shoot1.wav");
    Dod::BufferUtils::get(this->soundEngine.sounds, 1).load("resources/sounds/weapons/shoot2.wav");
}

void Game::ExecutionBlock::Sounds::update(float dt)
{

    const auto soundIdsToPlay{ Dod::SharedContext::get(this->sContext).soundIdsToPlay };
    for (int32_t sfxId{}; sfxId < Dod::BufferUtils::getNumFilledElements(soundIdsToPlay); ++sfxId) {
        this->soundEngine.soundsCore->play(Dod::BufferUtils::get(this->soundEngine.sounds, Dod::BufferUtils::get(soundIdsToPlay, sfxId)));
    }

}
