#include "BulletsCore.h"

#include <dod/BufferUtils.h>
#include <dod/Algorithms.h>

#include <algorithm>

void Game::Core::Bullets::updateLifetime(
    Dod::DBBuffer<int32_t>& elementsToDelete,
	Dod::DBBuffer<float>& positionsX, 
	Dod::DBBuffer<float>& positionsY
)
{

    std::sort(elementsToDelete.dataBegin + 1, elementsToDelete.dataBegin + 1 + Dod::BufferUtils::getNumFilledElements(elementsToDelete), std::greater());
    Dod::Algorithms::leftUniques(elementsToDelete);

    const auto bulletsToRemoveIm{ Dod::BufferUtils::createImFromBuffer(elementsToDelete) };

    Dod::BufferUtils::remove(positionsX, bulletsToRemoveIm);
    Dod::BufferUtils::remove(positionsY, bulletsToRemoveIm);

    elementsToDelete.numOfFilledEls = 0;

}

void Game::Core::Bullets::updateMovement(Dod::DBBuffer<float>& positions, float velocity, float dt)
{

    for (int32_t idx{ 0 }; idx < Dod::BufferUtils::getNumFilledElements(positions); ++idx) {
        Dod::BufferUtils::get(positions, idx) += velocity * dt;
    }

}
