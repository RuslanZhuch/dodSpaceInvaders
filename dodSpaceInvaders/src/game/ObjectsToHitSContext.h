#pragma once

#include <dod/BufferUtils.h>
#include <dod/MemPool.h>

namespace Game::Context::ObjectsToHit
{

	struct Shared
	{

		void init()
		{
			this->memory.allocate(1024 * 10);
			int32_t header{};
			constexpr int32_t totalElements{ 512 };
			Dod::BufferUtils::initFromMemory(this->objectsToHit, Dod::MemUtils::stackAquire(this->memory, totalElements * sizeof(float), header));
		}

		void reset()
		{
			Dod::BufferUtils::flush(this->objectsToHit);
		}

		void merge(const Shared& other)
		{
			Dod::BufferUtils::append(this->objectsToHit, Dod::BufferUtils::createImFromBuffer(other.objectsToHit));
		}

		Dod::MemPool memory;

		Dod::DBBuffer<int32_t> objectsToHit;

	};

};
