#pragma once

#include <dod/BufferUtils.h>

namespace Game::Core::Obstacles
{

	void updateLifetime(
		Dod::DBBuffer<int32_t>& obstaclesToRemove,
		Dod::DBBuffer<int32_t>& obstaclesToHit,
		Dod::DBBuffer<int32_t>& obstaclesLifes
	) noexcept;

	void remove(
		Dod::DBBuffer<int32_t>& obstaclesToRemove,
		Dod::DBBuffer<int32_t>& obstaclesLifes,
		Dod::DBBuffer<float>& obstacleCoordsX,
		Dod::DBBuffer<float>& obstacleCoordsY
	) noexcept;

};
