#pragma once

#include <dod/BufferUtils.h>
#include <cinttypes>

namespace Game::Gameplay::Player
{

    void updateInputs(
        uint32_t& inputBits,
        uint32_t& prevInputBits
    ) noexcept;

	void updateMovement(
        float& moveComponent,
        float& playerPositionX,
        uint32_t inputBits,
        uint32_t prevInputBits,
        float dt
    ) noexcept;

    int32_t updateFireComponent(
        int32_t playerLifes,
        uint32_t inputBits, 
        uint32_t prevInputBits
    ) noexcept;

    void createBulletsSFx(
        Dod::DBBuffer<int32_t>& soundIds,
        int32_t numOfBulletsCreated
    ) noexcept;

	void createBullets(
        int32_t numOfBulletsToCreate,
        Dod::DBBuffer<float>& bulletsXCoords,
        Dod::DBBuffer<float>& bulletsYCoords,
        float playerPositionX,
        float playerPositionY
    ) noexcept;

    void testWithBullets(
        Dod::DBBuffer<int32_t>& bulletsToDelete,
        Dod::ImBuffer<float> bulletsXCorrds,
        Dod::ImBuffer<float> bulletsYCorrds,
        int32_t& playerLifes,
        float playerX,
        float playerY,
        float playerWidth,
        float playerHeight
    ) noexcept;

    void lifetimeUpdate(
        Dod::ImBuffer<int32_t> enemiesToRemove,
        int32_t& lifes
    ) noexcept;
};
