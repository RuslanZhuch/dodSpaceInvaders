#pragma once

#include <dod/MemPool.h>
#include <dod/Buffers.h>

namespace Game::Context::SoundsController
{

	struct Data
	{
		void load() noexcept;
		void reset() noexcept;
		void merge(const Data& other) noexcept;

		Dod::MemPool memory;
		Dod::DBBuffer<int32_t> soundIdsToPlay;
	};

};
