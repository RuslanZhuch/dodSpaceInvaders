#pragma once

#include "GameRender.h"

#include <dod/MemPool.h>
#include <dod/BufferUtils.h>

namespace Game::Context::Models
{

	struct Shared
	{
		void init()
		{
			this->memory.allocate(1024);
			int32_t header{};
			constexpr int32_t totalElements{ 8 };
			Dod::BufferUtils::initFromMemory(this->loadedModels, Dod::MemUtils::stackAquire(this->memory, totalElements * sizeof(sf::VertexArray), header));
			Dod::BufferUtils::initFromMemory(this->modelNames, Dod::MemUtils::stackAquire(this->memory, totalElements * sizeof(std::array<char, 64>), header));
			Dod::BufferUtils::initFromMemory(this->modelIds, Dod::MemUtils::stackAquire(this->memory, totalElements * sizeof(int32_t), header));
		}

		void reset()
		{
			Dod::BufferUtils::flush(this->loadedModels);
			Dod::BufferUtils::flush(this->modelNames);
			Dod::BufferUtils::flush(this->modelIds);
		}

		void merge(const Shared& other)
		{
			Dod::BufferUtils::append(this->loadedModels, Dod::BufferUtils::createImFromBuffer(other.loadedModels));
			Dod::BufferUtils::append(this->modelNames, Dod::BufferUtils::createImFromBuffer(other.modelNames));
			Dod::BufferUtils::append(this->modelIds, Dod::BufferUtils::createImFromBuffer(other.modelIds));
		}

		Dod::MemPool memory;

		Dod::DBBuffer<sf::VertexArray> loadedModels;
		Dod::DBBuffer<std::array<char, 64>> modelNames;
		Dod::DBBuffer<int32_t> modelIds;

	};

};
