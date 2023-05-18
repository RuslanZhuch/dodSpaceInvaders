#pragma once

#include <dod/MemPool.h>
#include <dod/BufferUtils.h>

namespace Game::Context::Sounds
{

	struct Shared
	{
		void init()
		{
			this->memory.allocate(1024);
			int32_t header{};
			Dod::BufferUtils::initFromMemory(this->soundIdsToPlay, Dod::MemUtils::stackAquire(this->memory, 16 * sizeof(int32_t), header));
		}

		void reset()
		{
			Dod::BufferUtils::flush(this->soundIdsToPlay);
		}

		void merge(const Shared& other)
		{
			Dod::BufferUtils::append(this->soundIdsToPlay, Dod::BufferUtils::createImFromBuffer(other.soundIdsToPlay));
		}

		Dod::MemPool memory;
		Dod::DBBuffer<int32_t> soundIdsToPlay;
	};

};
