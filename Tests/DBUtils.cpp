#include "pch.h"

#include <dod/MemTypes.h>
#include <dod/DBBuffer.h>
#include <dod/DBUtils.h>

#include <array>
#include <ranges>

struct MemorySpan
{
	Dod::MemTypes::dataPoint_t dataBegin{};
	Dod::MemTypes::dataPoint_t dataEnd{};
};

TEST(DBUtils, Initialization)
{

	std::array<Dod::MemTypes::data_t, 32> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());


	{
		Dod::DBBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 32 };
		Dod::DBUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(buffer.numOfFilledEls, 0);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataPoint_t>(buffer.dataBegin), memSpan.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataPoint_t>(buffer.dataEnd), memSpan.dataBegin + endIndex);
	}
	{
		Dod::DBBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 16 };
		Dod::DBUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(buffer.numOfFilledEls, 0);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataPoint_t>(buffer.dataBegin), memSpan.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataPoint_t>(buffer.dataEnd), memSpan.dataBegin + endIndex);
	}
	{
		Dod::DBBuffer<int32_t> buffer;

		constexpr Dod::MemTypes::capacity_t beginIndex{ 8 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 24 };
		Dod::DBUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(buffer.numOfFilledEls, 0);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataPoint_t>(buffer.dataBegin), memSpan.dataBegin + beginIndex);
		EXPECT_EQ(reinterpret_cast<Dod::MemTypes::dataPoint_t>(buffer.dataEnd), memSpan.dataBegin + endIndex);
	}

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

		EXPECT_EQ(buffer.numOfFilledEls, 0);
		EXPECT_EQ(buffer.dataBegin, buffer.dataEnd);
	}

	{
		Dod::DBBuffer<int32_t> buffer;
		constexpr Dod::MemTypes::capacity_t beginIndex{ 35 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 40 };
		Dod::DBUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(buffer.numOfFilledEls, 0);
		EXPECT_EQ(buffer.dataBegin, buffer.dataEnd);
	}

	{
		Dod::DBBuffer<int32_t> buffer;
		constexpr Dod::MemTypes::capacity_t beginIndex{ 10 };
		constexpr Dod::MemTypes::capacity_t endIndex{ 5 };
		Dod::DBUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

		EXPECT_EQ(buffer.numOfFilledEls, 0);
		EXPECT_EQ(buffer.dataBegin, buffer.dataEnd);
	}

}

TEST(DBUtils, Population)
{

	std::array<Dod::MemTypes::data_t, 16> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBBuffer<int32_t> buffer;

	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ 16 };
	Dod::DBUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

	{
		const int32_t value{ 1 };
		Dod::DBUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(buffer.numOfFilledEls, 1);
	}
	{
		const int32_t value{ 2 };
		Dod::DBUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(buffer.numOfFilledEls, 2);
	}
	{
		const int32_t value{ 3 };
		Dod::DBUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 3), 3);
		EXPECT_EQ(buffer.numOfFilledEls, 3);
	}
	{
		const int32_t value{ 4 };
		Dod::DBUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 0), 4);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 3), 3);
		EXPECT_EQ(buffer.numOfFilledEls, 3);
	}

}

TEST(DBUtils, GetValue)
{

	using type_t = int32_t;
	constexpr size_t totalElements{ 8 };
	constexpr size_t totalBytes{ totalElements * sizeof(type_t) };
	std::array<type_t, totalElements> values{ {1, 2, 3, 4, 5, 6, 7, 8} };

	std::array<Dod::MemTypes::data_t, totalBytes> memory;
	std::memcpy(memory.data(), values.data(), memory.size());

	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBBuffer<int32_t> buffer;

	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ totalBytes };
	Dod::DBUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

	EXPECT_EQ(Dod::DBUtils::get(buffer, 0), static_cast<type_t>(2));
	EXPECT_EQ(Dod::DBUtils::get(buffer, 1), static_cast<type_t>(3));
	EXPECT_EQ(Dod::DBUtils::get(buffer, 2), static_cast<type_t>(4));
	EXPECT_EQ(Dod::DBUtils::get(buffer, 3), static_cast<type_t>(5));
	EXPECT_EQ(Dod::DBUtils::get(buffer, 4), static_cast<type_t>(6));
	EXPECT_EQ(Dod::DBUtils::get(buffer, 5), static_cast<type_t>(7));
	EXPECT_EQ(Dod::DBUtils::get(buffer, 6), static_cast<type_t>(8));

}

TEST(DBUtils, PopulationPartialMemory)
{

	std::array<Dod::MemTypes::data_t, 16> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBBuffer<int32_t> buffer;

	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ 13 };
	Dod::DBUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

	{
		const int32_t value{ 1 };
		Dod::DBUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(buffer.numOfFilledEls, 1);
	}
	{
		const int32_t value{ 2 };
		Dod::DBUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(buffer.numOfFilledEls, 2);
	}
	{
		const int32_t value{ 3 };
		Dod::DBUtils::populate(buffer, value, true);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 0), 3);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 1), 1);
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + 2), 2);
		EXPECT_EQ(buffer.numOfFilledEls, 2);
	}

}

TEST(DBUtils, PopulationConditional)
{

	using type_t = int32_t;

	constexpr size_t totalValues{ 1'000 };
	constexpr size_t totalMemory{ (totalValues + 1) * sizeof(type_t) };

	std::array<Dod::MemTypes::data_t, totalMemory> memory;
	MemorySpan memSpan(memory.data(), memory.data() + memory.size());

	Dod::DBBuffer<int32_t> buffer;
	constexpr Dod::MemTypes::capacity_t beginIndex{ 0 };
	constexpr Dod::MemTypes::capacity_t endIndex{ totalMemory };
	Dod::DBUtils::initFromMemory(buffer, memSpan, beginIndex, endIndex);

	for (type_t value{ 1 }; value < totalValues + 1; ++value)
	{
		const auto initialFilledEls{ buffer.numOfFilledEls };

		const auto bNeedAdd{ value % 2 == 0 };
		Dod::DBUtils::populate(buffer, value, bNeedAdd);

		const auto currentFilledEls{ buffer.numOfFilledEls };

		if (bNeedAdd)
			EXPECT_EQ(currentFilledEls - initialFilledEls, 1);
		else
			EXPECT_EQ(currentFilledEls - initialFilledEls, 0);

	}

	int32_t totalElements{ 0 };
	for (int32_t id{ 0 }; const auto value : std::ranges::views::iota(1, static_cast<int32_t>(totalValues) + 1)
		| std::ranges::views::filter([](auto val) { return val % 2 == 0; }))
	{
		EXPECT_EQ(*reinterpret_cast<int32_t*>(buffer.dataBegin + id + 1), value);
		++id;
		++totalElements;
	}
	EXPECT_EQ(totalElements, buffer.numOfFilledEls);

}