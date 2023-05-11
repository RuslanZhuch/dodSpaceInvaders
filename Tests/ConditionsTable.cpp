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

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(table.xOrMasks), totalRows);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(table.ignoreMasks), totalRows);

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

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(table.xOrMasks), totalRows);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(table.ignoreMasks), totalRows);

	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 0), 0x0000'0001);
	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 1), 0x0000'0003);
	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 2), 0x0000'0002);
	EXPECT_EQ(Dod::BufferUtils::get(table.xOrMasks, 3), 0x0000'000A);

	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 0), 0xFFFF'FFFE);
	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 1), 0xFFFF'FFFC);
	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 2), 0xFFFF'FFFC);
	EXPECT_EQ(Dod::BufferUtils::get(table.ignoreMasks, 3), 0xFFFF'FFF4);

}

TEST(ConditionTable, QueryPopulation)
{

	using type_t = int32_t;

	auto xOrMem{ std::to_array<uint32_t>({
		0x0000'0000,
		0x0000'0002,
		0x0000'0000,
		0x0000'0001,
		0x0000'0003,
	}) };

	auto ignoreMem{ std::to_array<uint32_t>({
		0x0000'0000,
		0xFFFF'FFFC,
		0xFFFF'FFFC,
		0xFFFF'FFFC,
		0xFFFF'FFFC,
	}) };

	Dod::DBBuffer<uint32_t> xOr;
	Dod::BufferUtils::initFromArray(xOr, xOrMem);
	xOr.numOfFilledEls = xOrMem.size() - 1;

	Dod::DBBuffer<uint32_t> ignore;
	Dod::BufferUtils::initFromArray(ignore, ignoreMem);
	ignore.numOfFilledEls = ignoreMem.size() - 1;

	const Dod::CondTable::Table table{
		Dod::BufferUtils::createImFromBuffer(xOr),
		Dod::BufferUtils::createImFromBuffer(ignore)
	};

	std::array<type_t, xOrMem.size()> quaryMem;
	Dod::DBBuffer<type_t> quary;
	Dod::BufferUtils::initFromArray(quary, quaryMem);

	{
		quary.numOfFilledEls = 0;
		type_t inputs{ 0x0000'0001 };
		Dod::CondTable::populateQuery(quary, inputs, table);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(quary), 1);
		EXPECT_EQ(Dod::BufferUtils::get(quary, 0), 0);
	}
	{
		quary.numOfFilledEls = 0;
		type_t inputs{ 0x0000'0003 };
		Dod::CondTable::populateQuery(quary, inputs, table);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(quary), 1);
		EXPECT_EQ(Dod::BufferUtils::get(quary, 0), 1);
	}
	{
		quary.numOfFilledEls = 0;
		type_t inputs{ 0x0000'0002 };
		Dod::CondTable::populateQuery(quary, inputs, table);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(quary), 1);
		EXPECT_EQ(Dod::BufferUtils::get(quary, 0), 2);
	}
	{
		quary.numOfFilledEls = 0;
		type_t inputs{ 0x0000'0000 };
		Dod::CondTable::populateQuery(quary, inputs, table);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(quary), 1);
		EXPECT_EQ(Dod::BufferUtils::get(quary, 0), 3);
	}

}

TEST(ConditionTable, QueryPopulationNoHit)
{

	using type_t = int32_t;

	auto xOrMem{ std::to_array<uint32_t>({
		0x0000'0000,
		0x0000'0002,
		0x0000'0000,
	}) };

	auto ignoreMem{ std::to_array<uint32_t>({
		0x0000'0000,
		0xFFFF'FFFC,
		0xFFFF'FFFC,
	}) };

	Dod::DBBuffer<uint32_t> xOr;
	Dod::BufferUtils::initFromArray(xOr, xOrMem);
	xOr.numOfFilledEls = xOrMem.size() - 1;

	Dod::DBBuffer<uint32_t> ignore;
	Dod::BufferUtils::initFromArray(ignore, ignoreMem);
	ignore.numOfFilledEls = ignoreMem.size() - 1;

	const Dod::CondTable::Table table{
		Dod::BufferUtils::createImFromBuffer(xOr),
		Dod::BufferUtils::createImFromBuffer(ignore)
	};

	std::array<type_t, xOrMem.size()> quaryMem;
	Dod::DBBuffer<type_t> quary;
	Dod::BufferUtils::initFromArray(quary, quaryMem);

	{
		quary.numOfFilledEls = 0;
		type_t inputs{ 0x0000'0001 };
		Dod::CondTable::populateQuery(quary, inputs, table);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(quary), 1);
		EXPECT_EQ(Dod::BufferUtils::get(quary, 0), 0);
	}
	{
		quary.numOfFilledEls = 0;
		type_t inputs{ 0x0000'0003 };
		Dod::CondTable::populateQuery(quary, inputs, table);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(quary), 1);
		EXPECT_EQ(Dod::BufferUtils::get(quary, 0), 1);
	}
	{
		quary.numOfFilledEls = 0;
		type_t inputs{ 0x0000'0002 };
		Dod::CondTable::populateQuery(quary, inputs, table);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(quary), 0);
	}
	{
		quary.numOfFilledEls = 0;
		type_t inputs{ 0x0000'0000 };
		Dod::CondTable::populateQuery(quary, inputs, table);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(quary), 0);
	}

}

TEST(ConditionTable, QueryPopulationWithSkips)
{

	using type_t = int32_t;

	auto xOrMem{ std::to_array<uint32_t>({
		0x0000'0000,
		0x0000'0000,
		0x0000'0000,
	}) };

	auto ignoreMem{ std::to_array<uint32_t>({
		0x0000'0000,
		0xFFFF'FFFD,
		0xFFFF'FFFF,
	}) };

	Dod::DBBuffer<uint32_t> xOr;
	Dod::BufferUtils::initFromArray(xOr, xOrMem);
	xOr.numOfFilledEls = xOrMem.size() - 1;

	Dod::DBBuffer<uint32_t> ignore;
	Dod::BufferUtils::initFromArray(ignore, ignoreMem);
	ignore.numOfFilledEls = ignoreMem.size() - 1;

	const Dod::CondTable::Table table{
		Dod::BufferUtils::createImFromBuffer(xOr),
		Dod::BufferUtils::createImFromBuffer(ignore)
	};

	std::array<type_t, xOrMem.size()> quaryMem;
	Dod::DBBuffer<type_t> quary;
	Dod::BufferUtils::initFromArray(quary, quaryMem);

	{
		quary.numOfFilledEls = 0;
		type_t inputs{ 0x0000'0002 };
		Dod::CondTable::populateQuery(quary, inputs, table);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(quary), 2);
		EXPECT_EQ(Dod::BufferUtils::get(quary, 0), 0);
		EXPECT_EQ(Dod::BufferUtils::get(quary, 1), 1);
	}
	{
		quary.numOfFilledEls = 0;
		type_t inputs{ 0x0000'0000 };
		Dod::CondTable::populateQuery(quary, inputs, table);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(quary), 1);
		EXPECT_EQ(Dod::BufferUtils::get(quary, 0), 1);
	}
	{
		quary.numOfFilledEls = 0;
		type_t inputs{ 0x0000'0003 };
		Dod::CondTable::populateQuery(quary, inputs, table);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(quary), 2);
		EXPECT_EQ(Dod::BufferUtils::get(quary, 0), 0);
		EXPECT_EQ(Dod::BufferUtils::get(quary, 1), 1);
	}
	{
		quary.numOfFilledEls = 0;
		type_t inputs{ 0x0000'0001 };
		Dod::CondTable::populateQuery(quary, inputs, table);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(quary), 1);
		EXPECT_EQ(Dod::BufferUtils::get(quary, 0), 1);
	}

}

TEST(ConditionTable, SimpleTransform)
{

	using type_t = float;

	type_t target{};
	const auto outputs{ std::to_array<type_t>({
		-2.f, -1.f, 1.f, 2.f
	}) };

	{
		target = 0.f;
		auto queryMem{ std::to_array<int32_t>({
			0, 0
		}) };
		Dod::DBBuffer<int32_t> query;
		Dod::BufferUtils::initFromArray(query, queryMem);
		query.numOfFilledEls = queryMem.size() - 1;

		Dod::CondTable::applyTransform<int32_t, type_t>(target, outputs, Dod::BufferUtils::createImFromBuffer(query));
		EXPECT_EQ(target, -2.f);
	}
	{
		target = 0.f;
		auto queryMem{ std::to_array<int32_t>({
			0, 1
		}) };
		Dod::DBBuffer<int32_t> query;
		Dod::BufferUtils::initFromArray(query, queryMem);
		query.numOfFilledEls = queryMem.size() - 1;

		Dod::CondTable::applyTransform<int32_t, type_t>(target, outputs, Dod::BufferUtils::createImFromBuffer(query));
		EXPECT_EQ(target, -1.f);
	}
	{
		target = 0.f;
		auto queryMem{ std::to_array<int32_t>({
			0, 3
		}) };
		Dod::DBBuffer<int32_t> query;
		Dod::BufferUtils::initFromArray(query, queryMem);
		query.numOfFilledEls = queryMem.size() - 1;

		Dod::CondTable::applyTransform<int32_t, type_t>(target, outputs, Dod::BufferUtils::createImFromBuffer(query));
		EXPECT_EQ(target, 2.f);
	}
	{
		target = 0.f;
		auto queryMem{ std::to_array<int32_t>({
			0
		}) };
		Dod::DBBuffer<int32_t> query;
		Dod::BufferUtils::initFromArray(query, queryMem);
		query.numOfFilledEls = queryMem.size() - 1;

		Dod::CondTable::applyTransform<int32_t, type_t>(target, outputs, Dod::BufferUtils::createImFromBuffer(query));
		EXPECT_EQ(target, 0.f);
	}
	{
		target = 0.f;
		auto queryMem{ std::to_array<int32_t>({
			0, 1, 3
		}) };
		Dod::DBBuffer<int32_t> query;
		Dod::BufferUtils::initFromArray(query, queryMem);
		query.numOfFilledEls = queryMem.size() - 1;

		Dod::CondTable::applyTransform<int32_t, type_t>(target, outputs, Dod::BufferUtils::createImFromBuffer(query));
		EXPECT_EQ(target, 2.f);
	}

}