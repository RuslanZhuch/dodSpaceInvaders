#pragma once

#include "MemTypes.h"
#include <vector>

namespace Dod
{

	template <typename T>
	struct DBBuffer
	{
		size_t numOfFilledEls{ 0 };
		T* dataBegin{ nullptr };
		T* dataEnd{ nullptr };
	};

};
