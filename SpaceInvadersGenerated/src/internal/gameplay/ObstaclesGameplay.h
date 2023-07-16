#pragma once

#include <dod/BufferUtils.h>

namespace Game::Gameplay::Obstacles
{

	void create(
        Dod::DBBuffer<float>& obstaclesX,
        Dod::DBBuffer<float>& obstaclesY,
        Dod::DBBuffer<int32_t>& obstaclesLifes,
        int32_t obstaclesPerRow,
        int32_t obstaclesPerCol,
        float obstaclesStride,
        int32_t obstaclesClusters,
        float obstaclesClustersTotalAreaWidth,
        float obstaclesClusterY,
        int32_t initialLifes,
        float width
	) noexcept;

};
