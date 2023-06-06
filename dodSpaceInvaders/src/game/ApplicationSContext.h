#pragma once

#include <dod/BufferUtils.h>
#include <dod/MemPool.h>

namespace Game::Context::Application
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
			this->memory.allocate(1024 * 10);
			int32_t header{};
			constexpr int32_t totalElements{ 512 };
			Dod::BufferUtils::initFromMemory(this->commands, Dod::MemUtils::stackAquire(this->memory, totalElements * sizeof(int32_t), header));
		}

		void reset()
		{
			Dod::BufferUtils::flush(this->commands);
		}

		void merge(const Shared& other)
		{
			Dod::BufferUtils::append(this->commands, Dod::BufferUtils::createImFromBuffer(other.commands));
		}

		Dod::MemPool memory;

		Dod::DBBuffer<int32_t> commands;

	};

};
