#pragma once

#include <dod/Buffers.h>

namespace Game::Enemies
{

	[[nodiscard]] bool computeNeedChangeDirection(
		float currentDirection,
		float currentXPosition,
		float mostLeft,
		float mostRight
	) noexcept;

	[[nodiscard]] float computeNewDirection(
		float currentDirection,
		bool strobe
	) noexcept;

	[[nodiscard]] float computeNewYPosition(
		float currentYPosition,
		float stride,
		bool strobe
	) noexcept;

	[[nodiscard]] float computeNewXPosition(
		float currentDirection,
		float currentXPosition,
		float stride,
		bool strobe
	) noexcept;

	[[nodiscard]] float updateStrobeCountdown(
		float dt,
		float prevTimeLeft,
		float initialTime
	) noexcept;

	[[nodiscard]] bool updateStrobe(
		float prevTimeLeft,
		float currentTime
	) noexcept;

	void generateX(
		Dod::DBBuffer<float>& xCoords,
		int32_t elementsPerRow,
		int32_t elementsPerCol,
		float offset,
		float stride
	) noexcept;

	void generateY(
		Dod::DBBuffer<float>& yCoords,
		int32_t elementsPerRow,
		int32_t elementsPerCol,
		float offset,
		float stride
	) noexcept;

};
