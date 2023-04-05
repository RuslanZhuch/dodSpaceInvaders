#pragma once

#include "MemTypes.h"
#include <vector>

namespace Dod
{

	template <typename T>
	struct DBBuffer
	{
		size_t size{ 0 };
		MemTypes::dataPoint_t dataBegin{ nullptr };
		MemTypes::dataPoint_t dataEnd{ nullptr };
	};

};
