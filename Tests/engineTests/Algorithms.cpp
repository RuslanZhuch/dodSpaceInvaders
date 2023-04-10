#include "pch.h"

#include <dod/Buffers.h>
#include <dod/BufferUtils.h>
#include <dod/Algorithms.h>

#include <array>

template <typename T>
static void initDBuffer(Dod::DBBuffer<T>& dest, auto& src)
{

	const size_t totalElements{ src.size() };
	const size_t totalBytes{ totalElements * sizeof(T) };

	std::memcpy(src.data(), src.data(), src.size());

	const Dod::MemTypes::capacity_t beginIndex{ 0 };
	const Dod::MemTypes::capacity_t endIndex{ totalBytes };

	struct MemorySpan
	{
		Dod::MemTypes::dataPoint_t dataBegin{};
		Dod::MemTypes::dataPoint_t dataEnd{};
	};
	MemorySpan memSpan(reinterpret_cast<Dod::MemTypes::dataPoint_t>(src.data()), Dod::MemTypes::dataPoint_t(src.data() + src.size()));
	Dod::BufferUtils::initFromMemory(dest, memSpan, beginIndex, endIndex);
	dest.numOfFilledEls = totalElements - 1;

}

TEST(Algorithms, LeftUniques)
{

	using type_t = int32_t;

	{
		auto values{ std::to_array<type_t>({0, 1, 1, 2, 2, 3, 3, 4, 4}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(buffer.numOfFilledEls, 4);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), 1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), 2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), 3);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 3), 4);
	}

	{
		auto values{ std::to_array<type_t>({0, 1, 1, 1, 1, 1, 1}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(buffer.numOfFilledEls, 1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), 1);
	}

	{
		auto values{ std::to_array<type_t>({0, 1, 2, 2, 2, 2, 2}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(buffer.numOfFilledEls, 2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), 1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), 2);
	}

	{
		auto values{ std::to_array<type_t>({0, 2, 2, 2, 2, 2, 1}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(buffer.numOfFilledEls, 2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), 2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), 1);
	}

	{
		auto values{ std::to_array<type_t>({0, 1, 2, 2, 2, 2, 3}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(buffer.numOfFilledEls, 3);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), 1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 1), 2);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 2), 3);
	}

	{
		auto values{ std::to_array<type_t>({0}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(buffer.numOfFilledEls, 0);
	}

	{
		auto values{ std::to_array<type_t>({0, 1}) };

		Dod::DBBuffer<type_t> buffer;
		initDBuffer(buffer, values);

		Dod::Algorithms::leftUniques(buffer);

		EXPECT_EQ(buffer.numOfFilledEls, 1);
		EXPECT_EQ(Dod::BufferUtils::get(buffer, 0), 1);
	}

}