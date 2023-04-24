#pragma once

#include <cinttypes>

namespace Game::Inputs
{

	[[nodiscard]] int32_t computeFireComponent(uint32_t currentInputs, uint32_t prevInputs) noexcept;
	[[nodiscard]] float computeMoveComponent(uint32_t currentInputs, uint32_t prevInputs, float prevMovement) noexcept;

}
