#include "SoundsExecutor.h"

void Game::ExecutionBlock::Sounds::loadContext()
{
}

void Game::ExecutionBlock::Sounds::initiate()
{
    this->soundsCore = std::make_unique<SoLoud::Soloud>();
    this->soundsCore->init();

    this->memory.allocate(sizeof(SoLoud::Wav) * 16);
    int32_t header{};
    Dod::BufferUtils::initFromMemory(this->sounds, Dod::MemUtils::stackAquire(this->memory, sizeof(SoLoud::Wav) * 16, header));

    Dod::BufferUtils::constructBack(this->sounds);
    Dod::BufferUtils::constructBack(this->sounds);
    Dod::BufferUtils::get(this->sounds, 0).load("resources/sounds/weapons/shoot1.wav");
    Dod::BufferUtils::get(this->sounds, 1).load("resources/sounds/weapons/shoot2.wav");
}

void Game::ExecutionBlock::Sounds::update(float dt)
{
    const auto soundIdsToPlay{ Dod::SharedContext::get(this->sContext).soundIdsToPlay };
    for (int32_t sfxId{}; sfxId < Dod::BufferUtils::getNumFilledElements(soundIdsToPlay); ++sfxId) {
        this->soundsCore->play(Dod::BufferUtils::get(this->sounds, Dod::BufferUtils::get(soundIdsToPlay, sfxId)));
    }

}
