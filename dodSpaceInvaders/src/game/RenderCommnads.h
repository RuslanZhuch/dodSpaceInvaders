#pragma once

#include <dod/BufferUtils.h>
#include <dod/MemPool.h>

namespace Game::Context::RenderCommands
{

	struct Data
	{

		struct ModelMeta
		{
			int32_t modelId{};
			int32_t numOfElements{};
		};

		void load() noexcept;
		void reset() noexcept;
		void merge(const Data& other) noexcept;

		Dod::MemPool memory;

		Dod::DBBuffer<float> xCoords;
		Dod::DBBuffer<float> yCoords;
		Dod::DBBuffer<ModelMeta> modelsMeta;

	};

};
