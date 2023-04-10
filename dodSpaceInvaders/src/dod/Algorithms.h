#pragma once

#include "dod/Buffers.h"

namespace Dod::Algorithms
{

	template <typename T>
	static void leftUniques(DBBuffer<T>& buffer) noexcept
	{

		if (buffer.numOfFilledEls == 0)
			return;

		T prevElValue{ buffer.dataBegin[1] };
		int32_t prevUniqueId{ 1 };
		for (int32_t elId{ 2 }; elId < buffer.numOfFilledEls + 1; ++elId)
		{
			const auto bIsUnique{ prevElValue != buffer.dataBegin[elId] };
			prevElValue = buffer.dataBegin[elId];
			if (bIsUnique)
			{
				const auto bGapExist{ elId - prevUniqueId >= 2 };
				if (bGapExist)
				{
					buffer.dataBegin[prevUniqueId + 1] = buffer.dataBegin[elId];
				}
				++prevUniqueId;
			}
		}

		buffer.numOfFilledEls = prevUniqueId;

	}

};
