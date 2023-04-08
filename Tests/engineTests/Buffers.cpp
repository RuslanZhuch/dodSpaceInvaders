#include "pch.h"

#include <dod/MemTypes.h>
#include <dod/Buffers.h>
#include <ranges>
#include <array>

TEST(Buffers, DBBuffer) 
{

	Dod::DBBuffer<int32_t> buffer;

	EXPECT_EQ(buffer.numOfFilledEls, 0);
	EXPECT_EQ(buffer.dataBegin, nullptr);
	EXPECT_EQ(buffer.dataEnd, nullptr);

}

TEST(Buffers, ImBuffer)
{

	Dod::ImBuffer<int32_t> buffer;

	EXPECT_EQ(buffer.numOfFilledEls, 0);
	EXPECT_EQ(buffer.dataBegin, nullptr);
	EXPECT_EQ(buffer.dataEnd, nullptr);

}
