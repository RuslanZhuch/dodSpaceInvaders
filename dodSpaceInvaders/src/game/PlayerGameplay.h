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

	void createBullets(
        Dod::DBBuffer<float>& bulletsXCoords,
        Dod::DBBuffer<float>& bulletsYCoords,
        float playerPositionX,
        float playerPositionY,
        int32_t playerLifes,
        uint32_t inputBits,
        uint32_t prevInputBits
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

};
