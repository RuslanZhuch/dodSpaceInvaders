#include "pch.h"

#include <dod/MemTypes.h>
#include <dod/DBBuffer.h>
#include <ranges>
#include <array>

TEST(DBBuffer, Creation) 
{

	Dod::DBBuffer<int32_t> buffer;

	EXPECT_EQ(buffer.numOfFilledEls, 0);
	EXPECT_EQ(buffer.dataBegin, nullptr);
	EXPECT_EQ(buffer.dataEnd, nullptr);

}

