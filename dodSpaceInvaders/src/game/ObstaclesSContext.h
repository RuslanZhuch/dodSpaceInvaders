#pragma once

#include <dod/BufferUtils.h>
#include <dod/MemPool.h>

namespace Game::Context::Obstacles
{

	struct Shared
	{

		void load()
		{
			this->memory.allocate(1024 * 10);
			int32_t header{};
			constexpr int32_t totalElements{ 512 };
			Dod::BufferUtils::initFromMemory(this->xCoords, Dod::MemUtils::stackAquire(this->memory, totalElements * sizeof(float), header));
			Dod::BufferUtils::initFromMemory(this->yCoords, Dod::MemUtils::stackAquire(this->memory, totalElements * sizeof(float), header));
		}

		void reset()
		{
			Dod::BufferUtils::flush(this->xCoords);
			Dod::BufferUtils::flush(this->yCoords);
		}

		void merge(const Shared& other)
		{
			Dod::BufferUtils::append(this->xCoords, Dod::BufferUtils::createImFromBuffer(other.xCoords));
			Dod::BufferUtils::append(this->yCoords, Dod::BufferUtils::createImFromBuffer(other.yCoords));
		}

		Dod::MemPool memory;

		Dod::DBBuffer<float> xCoords;
		Dod::DBBuffer<float> yCoords;

	};

};
