#include "pch.h"

#include <internal/gameplay/BulletsCore.cpp>

#include <dod/BufferUtils.h>

#include <array>

TEST(Bullets, bulletsLifetimeUpdate)
{

	constexpr auto totalElements{ 5 };

	std::array<float, totalElements + 1> positionsXMem;
	std::array<float, totalElements + 1> positionsYMem;
	Dod::DBBuffer<float> positionsX;
	Dod::BufferUtils::initFromArray(positionsX, positionsXMem);
	Dod::DBBuffer<float> positionsY;
	Dod::BufferUtils::initFromArray(positionsY, positionsYMem);
	
	std::array<float, totalElements + 1> elsToRemoveMem;
	Dod::DBBuffer<int32_t> elsToRemove;
	Dod::BufferUtils::initFromArray(elsToRemove, elsToRemoveMem);

	{
		positionsX.numOfFilledEls = 5;
		positionsY.numOfFilledEls = 5;
		Dod::BufferUtils::populate(elsToRemove, 0, true);
		Dod::BufferUtils::populate(elsToRemove, 1, true);
		Dod::BufferUtils::populate(elsToRemove, 2, true);
		Dod::BufferUtils::populate(elsToRemove, 3, true);
		Dod::BufferUtils::populate(elsToRemove, 4, true);

		Game::Core::Bullets::updateLifetime(elsToRemove, positionsX, positionsY);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(positionsX), 0);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(positionsY), 0);
	}

	{
		positionsX.numOfFilledEls = 5;
		positionsY.numOfFilledEls = 5;
		Dod::BufferUtils::populate(elsToRemove, 4, true);
		Dod::BufferUtils::populate(elsToRemove, 3, true);
		Dod::BufferUtils::populate(elsToRemove, 2, true);
		Dod::BufferUtils::populate(elsToRemove, 1, true);
		Dod::BufferUtils::populate(elsToRemove, 0, true);

		Game::Core::Bullets::updateLifetime(elsToRemove, positionsX, positionsY);

		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(positionsX), 0);
		EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(positionsY), 0);
	}

	{
		for (int32_t elId{ 0 }; elId < 5; ++elId)
		{
			Dod::BufferUtils::populate(positionsX, static_cast<float>((elId + 1)), true);
			Dod::BufferUtils::populate(positionsY, static_cast<float>((elId + 1) * 10.f), true);
		}
		positionsX.numOfFilledEls = 5;
		positionsY.numOfFilledEls = 5;

		Dod::BufferUtils::populate(elsToRemove, 3, true);
		Dod::BufferUtils::populate(elsToRemove, 4, true);
		Game::Core::Bullets::updateLifetime(elsToRemove, positionsX, positionsY);
		ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsX), 3);
		ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsY), 3);
		EXPECT_EQ(Dod::BufferUtils::get(positionsX, 0), 1.f);
		EXPECT_EQ(Dod::BufferUtils::get(positionsY, 0), 10.f);
		EXPECT_EQ(Dod::BufferUtils::get(positionsX, 1), 2.f);
		EXPECT_EQ(Dod::BufferUtils::get(positionsY, 1), 20.f);
		EXPECT_EQ(Dod::BufferUtils::get(positionsX, 2), 3.f);
		EXPECT_EQ(Dod::BufferUtils::get(positionsY, 2), 30.f);

		Dod::BufferUtils::populate(elsToRemove, 1, true);
		Game::Core::Bullets::updateLifetime(elsToRemove, positionsX, positionsY);
		ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsX), 2);
		ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsY), 2);
		EXPECT_EQ(Dod::BufferUtils::get(positionsX, 0), 1.f);
		EXPECT_EQ(Dod::BufferUtils::get(positionsY, 0), 10.f);
		EXPECT_EQ(Dod::BufferUtils::get(positionsX, 1), 3.f);
		EXPECT_EQ(Dod::BufferUtils::get(positionsY, 1), 30.f);

		Dod::BufferUtils::populate(elsToRemove, 0, true);
		Game::Core::Bullets::updateLifetime(elsToRemove, positionsX, positionsY);
		ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsX), 1);
		ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsY), 1);
		EXPECT_EQ(Dod::BufferUtils::get(positionsX, 0), 3.f);
		EXPECT_EQ(Dod::BufferUtils::get(positionsY, 0), 30.f);

		Dod::BufferUtils::populate(elsToRemove, 0, true);
		Game::Core::Bullets::updateLifetime(elsToRemove, positionsX, positionsY);
		ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsX), 0);
		ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsY), 0);
	}

	{
		for (int32_t elId{ 0 }; elId < 5; ++elId)
		{
			Dod::BufferUtils::populate(positionsX, static_cast<float>((elId + 1)), true);
			Dod::BufferUtils::populate(positionsY, static_cast<float>((elId + 1) * 10.f), true);
		}
		positionsX.numOfFilledEls = 5;
		positionsY.numOfFilledEls = 5;

		Dod::BufferUtils::populate(elsToRemove, 0, true);
		Dod::BufferUtils::populate(elsToRemove, 1, true);
		Dod::BufferUtils::populate(elsToRemove, 2, true);
		Game::Core::Bullets::updateLifetime(elsToRemove, positionsX, positionsY);
		ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsX), 2);
		ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsY), 2);
		EXPECT_EQ(Dod::BufferUtils::get(positionsX, 0), 5.f);
		EXPECT_EQ(Dod::BufferUtils::get(positionsY, 0), 50.f);
		EXPECT_EQ(Dod::BufferUtils::get(positionsX, 1), 4.f);
		EXPECT_EQ(Dod::BufferUtils::get(positionsY, 1), 40.f);

		Dod::BufferUtils::populate(elsToRemove, 1, true);
		Game::Core::Bullets::updateLifetime(elsToRemove, positionsX, positionsY);
		ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsX), 1);
		ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsY), 1);
		EXPECT_EQ(Dod::BufferUtils::get(positionsX, 0), 5.f);
		EXPECT_EQ(Dod::BufferUtils::get(positionsY, 0), 50.f);

		Dod::BufferUtils::populate(elsToRemove, 0, true);
		Game::Core::Bullets::updateLifetime(elsToRemove, positionsX, positionsY);
		ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsX), 0);
		ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsY), 0);
	}

}

class BulletsUpdateMovement : public ::testing::Test {

protected:
	static constexpr int32_t totalElements{ 5 };

	void SetUp() override
	{
		Dod::BufferUtils::initFromArray(positionsX, positionsXMem);
	}

	void initiatePoints(auto&& target, std::vector<float>&& data)
	{
		for (const auto el : data)
			Dod::BufferUtils::populate(target, el, true);
	}

	std::array<float, totalElements + 1> positionsXMem{};
	Dod::DBBuffer<float> positionsX;

};

TEST_F(BulletsUpdateMovement, initial)
{

	initiatePoints(positionsX, { 1.f, 2.f, 3.f, 4.f, 5.f });

	constexpr auto velocity{ 1.f };
	constexpr auto dt{ 2.f };

	Game::Core::Bullets::updateMovement(positionsX, velocity, dt);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsX), 5);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 0), 3.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 1), 4.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 2), 5.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 3), 6.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 4), 7.f);

	Game::Core::Bullets::updateMovement(positionsX, velocity, dt);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsX), 5);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 0), 5.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 1), 6.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 2), 7.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 3), 8.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 4), 9.f);

}

TEST_F(BulletsUpdateMovement, doubleVelocity)
{

	initiatePoints(positionsX, { 1.f, 2.f, 3.f, 4.f, 5.f });

	constexpr auto velocity{ 2.f };
	constexpr auto dt{ 2.f };

	Game::Core::Bullets::updateMovement(positionsX, velocity, dt);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsX), 5);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 0), 5.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 1), 6.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 2), 7.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 3), 8.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 4), 9.f);

}

TEST_F(BulletsUpdateMovement, increaseDt)
{

	initiatePoints(positionsX, { 1.f, 2.f, 3.f, 4.f, 5.f });

	constexpr auto velocity{ 1.f };
	constexpr auto dt{ 3.f };

	Game::Core::Bullets::updateMovement(positionsX, velocity, dt);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(positionsX), 5);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 0), 4.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 1), 5.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 2), 6.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 3), 7.f);
	EXPECT_EQ(Dod::BufferUtils::get(positionsX, 4), 8.f);

}
