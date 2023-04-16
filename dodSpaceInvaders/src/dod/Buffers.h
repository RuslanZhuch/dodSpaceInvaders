#pragma once

#include "MemTypes.h"
#include <vector>

namespace Dod
{

	template <typename T>
	struct DBBuffer
	{
		T* dataBegin{ nullptr };
		T* dataEnd{ nullptr };
		int32_t pad[3];
		int32_t numOfFilledEls{ 0 };
	};

	template <typename T>
	struct ImBuffer
	{
		const T* dataBegin{ nullptr };
		const T* dataEnd{ nullptr };
		int32_t pad[3];
		int32_t numOfFilledEls{ 0 };
	};

};
