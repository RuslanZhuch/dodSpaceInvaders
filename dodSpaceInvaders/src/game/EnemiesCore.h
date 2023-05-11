#pragma once

#include <dod/BufferUtils.h>

namespace Game::Core::Enemies
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

	void generateBullet(
		Dod::DBBuffer<float>& bulletPosition,
		Dod::ImBuffer<float> spawnPositions,
		auto& generator,
		bool strobe
	) noexcept
	{
		const auto numOfSources{ Dod::BufferUtils::getNumFilledElements(spawnPositions) };
		const auto spawnerId{ generator.generate(0, (numOfSources - 1) * (numOfSources > 0)) };

		Dod::BufferUtils::populate(bulletPosition, spawnPositions.dataBegin[spawnerId], strobe & (numOfSources > 0));
	}

	[[nodiscard]] bool fireRule(
		auto& generator,
		bool strobe
	) noexcept
	{
		const auto randValue{ generator.generate(0, 1000) };
		const auto bNeedCreateBullet{ randValue > 250 };

		return bNeedCreateBullet && strobe;
	}

};
