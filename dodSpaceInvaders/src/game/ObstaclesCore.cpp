#include "ObstaclesCore.h"

#include <dod/BufferUtils.h>

void Game::Core::Obstacles::updateLifetime(
	Dod::DBBuffer<int32_t>& obstaclesToRemove, 
	Dod::DBBuffer<int32_t>& obstaclesToHit, 
    Dod::DBBuffer<int32_t>& obstaclesLifes
) noexcept
{

    for (int32_t elId{ 0 }; elId < Dod::BufferUtils::getNumFilledElements(obstaclesToHit); ++elId)
    {
        const auto obstacleToHitId{ Dod::BufferUtils::get(obstaclesToHit, elId) };
        const auto lifesLeft{ --Dod::BufferUtils::get(obstaclesLifes, obstacleToHitId) };
        const auto bNeedToRemove{ lifesLeft <= 0 };

        Dod::BufferUtils::populate(obstaclesToRemove, obstacleToHitId, bNeedToRemove);
    }

    obstaclesToHit.numOfFilledEls = 0;

}

void Game::Core::Obstacles::remove(
    Dod::DBBuffer<int32_t>& obstaclesToRemove, 
    Dod::DBBuffer<int32_t>& obstaclesLifes, 
    Dod::DBBuffer<float>& obstacleCoordsX, 
    Dod::DBBuffer<float>& obstacleCoordsY
) noexcept
{

    const auto obstaclesToRemoveIm{ Dod::BufferUtils::createImFromBuffer(obstaclesToRemove) };
    Dod::BufferUtils::remove(obstaclesLifes, obstaclesToRemoveIm);
    Dod::BufferUtils::remove(obstacleCoordsX, obstaclesToRemoveIm);
    Dod::BufferUtils::remove(obstacleCoordsY, obstaclesToRemoveIm);
    
    obstaclesToRemove.numOfFilledEls = 0;

}
