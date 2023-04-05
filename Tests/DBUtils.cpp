#include "pch.h"

#include <dod/MemTypes.h>
#include <dod/DBBuffer.h>
#include <dod/DBUtils.h>

#include <array>

struct MemorySpan
{
	Dod::MemTypes::dataPoint_t dataBegin{};
	Dod::MemTypes::dataPoint_t dataEnd{};
};

TEST(DBUtils, Initialization)
{

	std::array<Dod::MemTypes::data_t, 32> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBBuffer<int32_t> buffer;

	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ 10 };
	Dod::DBUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

	EXPECT_EQ(buffer.size, 10);
	EXPECT_EQ(buffer.dataBegin, memSpan.dataBegin);
	EXPECT_EQ(buffer.dataEnd, memSpan.dataBegin + endIndex);

}

TEST(DBUtils, InitializationFailed)
{

	std::array<Dod::MemTypes::data_t, 32> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	{
		Dod::DBBuffer<int32_t> buffer;
		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 40 };
		Dod::DBUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(buffer.size, 0);
		EXPECT_EQ(buffer.dataBegin, buffer.dataEnd);
	}

	{
		Dod::DBBuffer<int32_t> buffer;
		constexpr Dod::MemTypes::capacity_t beginIndex{ 35 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 40 };
		Dod::DBUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(buffer.size, 0);
		EXPECT_EQ(buffer.dataBegin, buffer.dataEnd);
	}

	{
		Dod::DBBuffer<int32_t> buffer;
		constexpr Dod::MemTypes::capacity_t beginIndex{ 10 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 5 };
		Dod::DBUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(buffer.size, 0);
		EXPECT_EQ(buffer.dataBegin, buffer.dataEnd);
	}

}