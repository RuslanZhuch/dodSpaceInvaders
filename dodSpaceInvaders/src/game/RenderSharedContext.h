#pragma once

#include <dod/BufferUtils.h>

namespace Game::Context::Render
{

	struct Shared
	{

		struct ModelMeta
		{
			int32_t modelId{};
			int32_t numOfElements{};
		};

		void init()
		{
			this->memory.allocate(1024);
			int32_t header{};
			constexpr int32_t totalElements{ 512 };
			Dod::BufferUtils::initFromMemory(this->xCoords, Dod::MemUtils::stackAquire(this->memory, totalElements * sizeof(float), header));
			Dod::BufferUtils::initFromMemory(this->yCoords, Dod::MemUtils::stackAquire(this->memory, totalElements * sizeof(float), header));
			Dod::BufferUtils::initFromMemory(this->modelsMeta, Dod::MemUtils::stackAquire(this->memory, totalElements * sizeof(ModelMeta), header));
		}

		void reset()
		{
			Dod::BufferUtils::flush(this->xCoords);
			Dod::BufferUtils::flush(this->yCoords);
			Dod::BufferUtils::flush(this->modelsMeta);
		}

		void merge(const Shared& other)
		{
			Dod::BufferUtils::append(this->xCoords, Dod::BufferUtils::createImFromBuffer(other.xCoords));
			Dod::BufferUtils::append(this->yCoords, Dod::BufferUtils::createImFromBuffer(other.yCoords));
			Dod::BufferUtils::append(this->modelsMeta, Dod::BufferUtils::createImFromBuffer(other.modelsMeta));
		}

		Dod::MemPool memory;

		Dod::DBBuffer<float> xCoords;
		Dod::DBBuffer<float> yCoords;
		Dod::DBBuffer<ModelMeta> modelsMeta;

	};

};
