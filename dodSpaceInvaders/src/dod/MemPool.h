#pragma once

#include "MemTypes.h"

#include <vector>

namespace Dod
{

	struct MemPool
	{


		explicit MemPool(MemTypes::capacity_t capacityInBytes) noexcept;

		MemTypes::dataPoint_t dataBegin{ nullptr };
		MemTypes::dataPoint_t dataEnd{ nullptr };

	private:
		std::vector<MemTypes::data_t> data;
		
	};

};
