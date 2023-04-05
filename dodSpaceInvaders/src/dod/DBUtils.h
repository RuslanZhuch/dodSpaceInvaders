#pragma once

#include "MemTypes.h"
#include "MemUtils.h"

namespace Dod::DBUtils
{

	void initFromMemory(auto& dbBuffer, const auto& memSpan, MemTypes::capacity_t beginIndex, MemTypes::capacity_t endIndex) noexcept
	{

		const auto actualData{ MemUtils::aquire(memSpan, beginIndex, endIndex) };

		dbBuffer.dataBegin = actualData.dataBegin;
		dbBuffer.dataEnd = actualData.dataEnd;
		dbBuffer.size = actualData.dataEnd - actualData.dataBegin;

	}

};
