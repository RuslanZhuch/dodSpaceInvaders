#include "pch.h"
#include <dod/MemPool.h>
#include <dod/MemPool.cpp>

#pragma warning(push)
#pragma warning(disable : 4365)

#include <array>
#pragma warning(pop)

TEST(MemPool, generation)
{

	const auto bytesToAllocate{ std::to_array({
		10, 100, 1000, 1'000'000
	}) };

	for (const auto bytes : bytesToAllocate)
	{
		Dod::MemPool pool(bytes);
		const auto begin{ pool.dataBegin };
		const auto end{ pool.dataEnd };
		EXPECT_NE(begin, nullptr);
		EXPECT_NE(end, nullptr);
		EXPECT_EQ(end - begin, bytes);
	}

	{
		Dod::MemPool pool(0);
		const auto begin{ pool.dataBegin };
		const auto end{ pool.dataEnd };
		EXPECT_EQ(begin, nullptr);
		EXPECT_EQ(end, nullptr);
		EXPECT_EQ(end - begin, 0);
	}

}

//TEST(MemPool, Acquiring)
//{
//
//	Dod::MemPool pool(100);
//
//	const auto ptr1{ pool.aquire(10) };
//	EXPECT_NE(ptr1, nullptr);
//
//	const auto ptr2{ pool.aquire(20) };
//	EXPECT_NE(ptr2, nullptr);
//	EXPECT_EQ(ptr2 - ptr1, 10);
//
//	const auto ptr3{ pool.aquire(30) };
//	EXPECT_NE(ptr3, nullptr);
//	EXPECT_EQ(ptr3 - ptr2, 20);
//
//	{
//		const auto ptr{ pool.aquire(60) };
//		EXPECT_EQ(ptr, nullptr);
//	}
//
//	const auto ptr4{ pool.aquire(40) };
//	EXPECT_NE(ptr4, nullptr);
//	EXPECT_EQ(ptr4 - ptr3, 30);
//
//	{
//		const auto ptr{ pool.aquire(1) };
//		EXPECT_EQ(ptr, nullptr);
//	}
//
//}