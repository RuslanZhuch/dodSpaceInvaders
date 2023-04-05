#include "pch.h"

#include <dod/MemTypes.h>
#include <dod/DBBuffer.h>
#include <ranges>
#include <array>

TEST(DBBuffer, Creation) 
{

	Dod::DBBuffer<int32_t> buffer;

	EXPECT_EQ(buffer.size, 0);
	EXPECT_EQ(buffer.dataBegin, nullptr);
	EXPECT_EQ(buffer.dataEnd, nullptr);

}

//TEST(DBBuffer, Population) {
//
//	Dod::DBBuffer<int32_t> buffer;
//
//	constexpr auto numOfValues{ 1'000 };
//	for (int32_t value{ 1 }; value < numOfValues + 1; ++value)
//	{
//		const auto initialSize{ buffer.size };
//		const auto initialVSize{ buffer.vsize };
//		
//		const auto bNeedAdd{ value % 2 == 0 };
//		Dod::populate(buffer, value, bNeedAdd);
//
//		const auto currentSize{ buffer.size };
//		const auto currentVSize{ buffer.vsize };
//
//		if (bNeedAdd)
//		{
//			EXPECT_EQ(currentSize - initialSize, 1);
//			EXPECT_EQ(currentVSize - initialVSize, 1);
//		}
//		else
//		{
//			EXPECT_EQ(currentSize - initialSize, 0);
//			EXPECT_EQ(currentVSize - initialVSize, 0);
//		}
//
//	}
//
//	int32_t totalElements{ 0 };
//	for (int32_t id{ 0 }; const auto value : std::ranges::views::iota(1, numOfValues + 1)
//		| std::ranges::views::filter([](auto val) { return val % 2 == 0; }))
//	{
//		EXPECT_EQ(Dod::get(buffer, id), value);
//		++id;
//		++totalElements;
//	}
//	EXPECT_EQ(totalElements, buffer.vsize);
//
//}
