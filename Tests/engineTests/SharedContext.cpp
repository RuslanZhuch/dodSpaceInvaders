#include "pch.h"

#include <dod/BufferUtils.h>
#include <dod/SharedContext.h>

#include <array>

struct Context1
{
	float data1{};
	float data2{};

	Dod::DBBuffer<int32_t> buffer1;
	std::array<int32_t, 10> buffer1Memory{};

	void init()
	{
		this->data1 = 42.f;
		this->data2 = 123.f;

		this->buffer1Memory = std::to_array<int32_t>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
		Dod::BufferUtils::initFromArray(this->buffer1, this->buffer1Memory);
		this->buffer1.numOfFilledEls = this->buffer1Memory.size() - 1;
	}

	void reset()
	{
		Dod::BufferUtils::flush(this->buffer1);
	}

	void merge(const Context1& other)
	{
		Dod::BufferUtils::append(this->buffer1, Dod::BufferUtils::createImFromBuffer(other.buffer1));
	}
};

TEST(SharedContext, readData)
{

	Dod::SharedContext::Controller<Context1> controller;

	EXPECT_EQ(Dod::SharedContext::get(&controller).data1, 42.f);
	EXPECT_EQ(Dod::SharedContext::get(&controller).data2, 123.f);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(Dod::SharedContext::get(&controller).buffer1), 9);

	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 0), 2);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 1), 3);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 2), 4);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 3), 5);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 4), 6);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 5), 7);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 6), 8);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 7), 9);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 8), 10);

}

TEST(SharedContext, flushData)
{

	Dod::SharedContext::Controller<Context1> controller;
	Dod::SharedContext::flush(&controller);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(Dod::SharedContext::get(&controller).buffer1), 0);

}

TEST(SharedContext, mergeData)
{

	Dod::SharedContext::Controller<Context1> controller;
	Dod::SharedContext::flush(&controller);
	for (int32_t elId{}; elId < 1; ++elId)
		Dod::BufferUtils::populate(controller.context.buffer1, elId + 1, true);

	Dod::SharedContext::Controller<Context1> localContext1;
	Dod::SharedContext::flush(&localContext1);
	for (int32_t elId{}; elId < 3; ++elId)
		Dod::BufferUtils::populate(localContext1.context.buffer1, elId + 2, true);

	Dod::SharedContext::Controller<Context1> localContext2;
	Dod::SharedContext::flush(&localContext2);
	for (int32_t elId{}; elId < 4; ++elId)
		Dod::BufferUtils::populate(localContext2.context.buffer1, elId + 5, true);

	Dod::SharedContext::Controller<Context1> localContext3;
	Dod::SharedContext::flush(&localContext3);

	Dod::SharedContext::merge(&controller, &localContext1);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(controller.context.buffer1), 4);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 0), 1);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 1), 2);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 2), 3);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 3), 4);

	Dod::SharedContext::merge(&controller, &localContext3);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(controller.context.buffer1), 4);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 0), 1);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 1), 2);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 2), 3);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 3), 4);

	Dod::SharedContext::merge(&controller, &localContext2);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(controller.context.buffer1), 8);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 0), 1);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 1), 2);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 2), 3);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 3), 4);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 4), 5);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 5), 6);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 6), 7);
	EXPECT_EQ(Dod::BufferUtils::get(Dod::SharedContext::get(&controller).buffer1, 7), 8);

}
