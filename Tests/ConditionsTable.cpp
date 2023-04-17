#include "pch.h"

#include <dod/BufferUtils.h>
#include <dod/ConditionTable.h>

#include <array>
#include <vector>

TEST(ConditionTable, Generation)
{

	constexpr auto totalRows{ 9 };

	std::array<std::array<Dod::CondTable::TriState, 2>, totalRows> tableSrc{
		{
			{Dod::CondTable::TriState::TRUE, Dod::CondTable::TriState::FALSE},
			{Dod::CondTable::TriState::TRUE, Dod::CondTable::TriState::TRUE},
			{Dod::CondTable::TriState::FALSE, Dod::CondTable::TriState::TRUE},
			{Dod::CondTable::TriState::FALSE, Dod::CondTable::TriState::FALSE},
			{Dod::CondTable::TriState::TRUE, Dod::CondTable::TriState::SKIP},
			{Dod::CondTable::TriState::SKIP, Dod::CondTable::TriState::TRUE},
			{Dod::CondTable::TriState::SKIP, Dod::CondTable::TriState::SKIP},
			{Dod::CondTable::TriState::FALSE, Dod::CondTable::TriState::SKIP},
			{Dod::CondTable::TriState::SKIP, Dod::CondTable::TriState::FALSE},
		}
	};

	using type_t = int32_t;

	std::array<uint32_t, totalRows + 1> xOrMem;
	std::array<uint32_t, totalRows + 1> ignoreMem;
	Dod::DBBuffer<int32_t> srcBuffer;

	Dod::CondTable::Table table{ Dod::CondTable::generate(tableSrc, xOrMem, ignoreMem) };

	EXPECT_EQ(table.xOrMasks.numOfFilledEls, totalRows);
	EXPECT_EQ(table.ignoreMasks.numOfFilledEls, totalRows);

	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 0), 0x0000'0002);
	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 1), 0x0000'0000);
	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 2), 0x0000'0001);
	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 3), 0x0000'0003);
	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 4), 0x0000'0000);
	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 5), 0x0000'0000);
	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 6), 0x0000'0000);
	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 7), 0x0000'0001);
	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 8), 0x0000'0002);

	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 0), 0xFFFF'FFFC);
	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 1), 0xFFFF'FFFC);
	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 2), 0xFFFF'FFFC);
	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 3), 0xFFFF'FFFC);
	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 4), 0xFFFF'FFFE);
	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 5), 0xFFFF'FFFD);
	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 6), 0xFFFF'FFFF);
	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 7), 0xFFFF'FFFE);
	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 8), 0xFFFF'FFFD);

}

TEST(ConditionTable, GenerationNonUniform)
{

	constexpr auto totalRows{ 4 };

	std::array<std::vector<Dod::CondTable::TriState>, totalRows> tableSrc{
		{
			{Dod::CondTable::TriState::FALSE, },
			{Dod::CondTable::TriState::FALSE, Dod::CondTable::TriState::FALSE},
			{Dod::CondTable::TriState::TRUE, Dod::CondTable::TriState::FALSE, Dod::CondTable::TriState::SKIP },
			{Dod::CondTable::TriState::TRUE, Dod::CondTable::TriState::FALSE, Dod::CondTable::TriState::SKIP, Dod::CondTable::TriState::FALSE },
		}
	};

	using type_t = int32_t;
	
	std::array<uint32_t, totalRows + 1> xOrMem;
	std::array<uint32_t, totalRows + 1> ignoreMem;
	Dod::DBBuffer<int32_t> srcBuffer;

	Dod::CondTable::Table table{ Dod::CondTable::generate(tableSrc, xOrMem, ignoreMem) };

	EXPECT_EQ(table.xOrMasks.numOfFilledEls, totalRows);
	EXPECT_EQ(table.ignoreMasks.numOfFilledEls, totalRows);

	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 0), 0x0000'0001);
	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 1), 0x0000'0003);
	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 2), 0x0000'0002);
	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 3), 0x0000'000A);

	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 0), 0xFFFF'FFFE);
	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 1), 0xFFFF'FFFC);
	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 2), 0xFFFF'FFFC);
	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 3), 0xFFFF'FFF4);

}