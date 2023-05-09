#pragma once

#include <dod/BufferUtils.h>

namespace Game::Core::Bullets
{

	void updateLifetime(Dod::DBBuffer<int32_t>& elementsToDelete, Dod::DBBuffer<float>& positionsX, Dod::DBBuffer<float>& positionsY);
	void updateMovement(Dod::DBBuffer<float>& positions, float velocity, float dt);

};
