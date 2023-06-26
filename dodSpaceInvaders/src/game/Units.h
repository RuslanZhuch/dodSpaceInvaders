#pragma once

#include <dod/BufferUtils.h>
#include <dod/MemPool.h>

namespace Game::Context::Units
{

	struct Data
	{

		void load();

		void reset();

		void merge(const Data& other);

		Dod::MemPool memory;

		Dod::DBBuffer<float> xCoords;
		Dod::DBBuffer<float> yCoords;
		float width{};
		float height{};

	};

};
