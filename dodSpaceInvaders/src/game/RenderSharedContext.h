#pragma once

#include <dod/BufferUtils.h>

namespace Game::Context::Render
{

	struct Shared
	{

		struct Vec2
		{
			float x{};
			float y{};
		};

		void init()
		{
			this->memory.allocate(1024);
			int32_t header{};
			constexpr int32_t totalElements{ 512 };
			Dod::BufferUtils::initFromMemory(this->centers, Dod::MemUtils::stackAquire(this->memory, totalElements * sizeof(Vec2), header));
			Dod::BufferUtils::initFromMemory(this->modelIds, Dod::MemUtils::stackAquire(this->memory, totalElements * sizeof(int32_t), header));
		}

		void reset()
		{
			Dod::BufferUtils::flush(this->centers);
			Dod::BufferUtils::flush(this->modelIds);
		}

		void merge(const Shared& other)
		{
			Dod::BufferUtils::append(this->centers, Dod::BufferUtils::createImFromBuffer(other.centers));
			Dod::BufferUtils::append(this->modelIds, Dod::BufferUtils::createImFromBuffer(other.modelIds));
		}

		Dod::MemPool memory;

		Dod::DBBuffer<Vec2> centers;
		Dod::DBBuffer<int32_t> modelIds;

	};

};
