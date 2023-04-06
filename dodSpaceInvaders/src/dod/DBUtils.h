#pragma once

#include "MemTypes.h"
#include "MemUtils.h"

#include <type_traits>

namespace Dod::DBUtils
{

	void initFromMemory(auto& dbBuffer, const auto& memSpan, MemTypes::capacity_t beginIndex, MemTypes::capacity_t endIndex) noexcept
	{

		const auto actualData{ MemUtils::aquire(memSpan, beginIndex, endIndex) };

		//TODO: Is it legal?
		dbBuffer.dataBegin = reinterpret_cast<decltype(dbBuffer.dataBegin)>(actualData.dataBegin);
		dbBuffer.dataEnd = reinterpret_cast<decltype(dbBuffer.dataEnd)>(actualData.dataEnd);
		dbBuffer.numOfFilledEls = 0;

	}

	void populate(auto& buffer, auto value, bool strobe) noexcept
	{

		using type_t = decltype(std::remove_pointer_t<decltype(buffer.dataBegin)>);

		const auto capacity{ buffer.dataEnd - buffer.dataBegin };
		const auto bCanAddValue{ (buffer.numOfFilledEls + 1 < capacity) && strobe };

		buffer.numOfFilledEls += size_t(1) * bCanAddValue;
		buffer.dataBegin[buffer.numOfFilledEls * bCanAddValue] = value;

	}

	[[nodiscard]] auto get(const auto& buffer, int32_t elId) noexcept
	{

		using type_t = decltype(std::remove_pointer_t<decltype(buffer.dataBegin)>);

		return buffer.dataBegin[elId + 1];

	}

};
