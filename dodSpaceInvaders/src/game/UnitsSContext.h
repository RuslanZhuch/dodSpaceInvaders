#pragma once

#include <dod/BufferUtils.h>
#include <dod/MemPool.h>

namespace Game::Context::Units
{

	struct Data
	{

		void load() noexcept;

		void reset() noexcept;
		void merge(const Data& other) noexcept;

		Dod::MemPool memory;

		Dod::DBBuffer<float> xCoords;
		Dod::DBBuffer<float> yCoords;
		Dod::DBBuffer<float> groupWidth{};
		Dod::DBBuffer<float> groupHeight{};
		Dod::DBBuffer<int32_t> elementsInGroup{};

	};

};
