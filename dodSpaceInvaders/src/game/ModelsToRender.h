#pragma once

#include "GameRender.h"

#include <dod/MemPool.h>
#include <dod/BufferUtils.h>

#include <array>

namespace Game::Context::ModelsToRender
{

	struct Data
	{
		void load() noexcept;
		void reset() noexcept;
		void merge(const Data& other) noexcept;

		Dod::MemPool memory;

		Dod::DBBuffer<sf::VertexArray> loadedModels;
		Dod::DBBuffer<std::array<char, 64>> modelNames;
		Dod::DBBuffer<int32_t> modelIds;

	};

};
