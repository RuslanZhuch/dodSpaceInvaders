#pragma once

#include <dod/BufferUtils.h>
#include <dod/MemPool.h>

namespace Game::Context::ObjectsToHit
{

	struct Data
	{

		void load() noexcept;
		void reset() noexcept;
		void merge(const Data& other) noexcept;

		Dod::MemPool memory;
		Dod::DBBuffer<int32_t> objectsToHit;

	};

};
