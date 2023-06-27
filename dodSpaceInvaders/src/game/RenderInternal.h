#pragma once

#include "GameRender.h"

#include <dod/BufferUtils.h>
#include <dod/MemPool.h>

namespace Game::Context::RenderInternal
{

	struct Data
	{

		void load() noexcept;
		void reset() noexcept;
		void merge(const Data& other) noexcept;

		Dod::MemPool memory;

		std::unique_ptr<GameRenderer> gameRenderer;
		Dod::DBBuffer<int32_t> idsToRender;

	};

};
