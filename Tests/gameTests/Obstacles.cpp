#include "pch.h"

#include <dod/BufferUtils.h>

#include <game/ObstaclesCore.cpp>

#include <array>

class ObstaclesLifetimeUpdate : public ::testing::Test {

protected:
	static constexpr int32_t totalElements{ 3 };

	void SetUp() override
	{
		Dod::BufferUtils::initFromArray(obstaclesLifes, obstaclesLifesMem);
		Dod::BufferUtils::initFromArray(obstaclesToRemove, obstaclesToRemoveMem);
		Dod::BufferUtils::initFromArray(obstaclesToHit, obstaclesToHitMem);
	}

	void populate(auto&& target, std::vector<int32_t>&& data)
	{
		for (const auto el : data)
			Dod::BufferUtils::populate(target, el, true);
	}

	std::array<int32_t, totalElements + 1> obstaclesLifesMem{};
	std::array<int32_t, totalElements + 1> obstaclesToRemoveMem{};
	std::array<int32_t, totalElements + 1> obstaclesToHitMem{};
	Dod::DBBuffer<int32_t> obstaclesLifes;
	Dod::DBBuffer<int32_t> obstaclesToRemove;
	Dod::DBBuffer<int32_t> obstaclesToHit;

};

TEST_F(ObstaclesLifetimeUpdate, all)
{

	populate(obstaclesLifes, { 0, 0, 0 });
	populate(obstaclesToHit, { 0, 1, 2 });

	Game::Core::Obstacles::updateLifetime(obstaclesToRemove, obstaclesToHit, obstaclesLifes);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 3); 
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToHit), 0);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesToRemove, 0), 0);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesToRemove, 1), 1);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesToRemove, 2), 2);

}

TEST_F(ObstaclesLifetimeUpdate, none)
{

	populate(obstaclesLifes, { 0, 0, 0 });

	Game::Core::Obstacles::updateLifetime(obstaclesToRemove, obstaclesToHit, obstaclesLifes);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToHit), 0);

}

TEST_F(ObstaclesLifetimeUpdate, partial)
{

	populate(obstaclesLifes, { 0, 0, 0 });
	populate(obstaclesToHit, { 0, 2 });

	Game::Core::Obstacles::updateLifetime(obstaclesToRemove, obstaclesToHit, obstaclesLifes);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 2);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToHit), 0);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesToRemove, 0), 0);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesToRemove, 1), 2);

}

TEST_F(ObstaclesLifetimeUpdate, allInverted)
{

	populate(obstaclesLifes, { 0, 0, 0 });
	populate(obstaclesToHit, { 2, 1, 0 });

	Game::Core::Obstacles::updateLifetime(obstaclesToRemove, obstaclesToHit, obstaclesLifes);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 3);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToHit), 0);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesToRemove, 0), 2);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesToRemove, 1), 1);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesToRemove, 2), 0);

}

TEST_F(ObstaclesLifetimeUpdate, allMixed)
{

	populate(obstaclesLifes, { 0, 0, 0 });
	populate(obstaclesToHit, { 2, 0, 1 });

	Game::Core::Obstacles::updateLifetime(obstaclesToRemove, obstaclesToHit, obstaclesLifes);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 3);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToHit), 0);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesToRemove, 0), 2);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesToRemove, 1), 0);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesToRemove, 2), 1);

}

TEST_F(ObstaclesLifetimeUpdate, allLifes)
{

	populate(obstaclesLifes, { 2, 1, 0 });
	populate(obstaclesToHit, { 0, 1, 2 });

	Game::Core::Obstacles::updateLifetime(obstaclesToRemove, obstaclesToHit, obstaclesLifes);

	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 2);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToHit), 0);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesToRemove, 0), 1);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesToRemove, 1), 2);

}

class ObstaclesRemove : public ::testing::Test {

protected:
	static constexpr int32_t totalElements{ 3 };

	void SetUp() override
	{
		Dod::BufferUtils::initFromArray(obstaclesLifes, obstaclesLifesMem);
		Dod::BufferUtils::initFromArray(obstaclesToRemove, obstaclesToRemoveMem);
		Dod::BufferUtils::initFromArray(obstaclesXCoords, obstaclesXCoordsMem);
		Dod::BufferUtils::initFromArray(obstaclesYCoords, obstaclesYCoordsMem);
	}

	void populate(auto&& target, std::vector<int32_t>&& data)
	{
		for (const auto el : data)
			Dod::BufferUtils::populate(target, el, true);
	}
	void populateCoords(auto&& target, std::vector<float>&& data)
	{
		for (const auto el : data)
			Dod::BufferUtils::populate(target, el, true);
	}

	std::array<int32_t, totalElements + 1> obstaclesLifesMem{};
	std::array<int32_t, totalElements + 1> obstaclesToRemoveMem{};
	std::array<float, totalElements + 1> obstaclesXCoordsMem{};
	std::array<float, totalElements + 1> obstaclesYCoordsMem{};

	Dod::DBBuffer<int32_t> obstaclesLifes;
	Dod::DBBuffer<int32_t> obstaclesToRemove;
	Dod::DBBuffer<float> obstaclesXCoords;
	Dod::DBBuffer<float> obstaclesYCoords;

};

TEST_F(ObstaclesRemove, all)
{

	populate(obstaclesLifes, { 1, 1, 1 });
	populate(obstaclesToRemove, { 0, 1, 2 });
	populateCoords(obstaclesXCoords, { 10.f, 20.f, 30.f });
	populateCoords(obstaclesYCoords, { 40.f, 50.f, 60.f });

	Game::Core::Obstacles::remove(obstaclesToRemove, obstaclesLifes, obstaclesXCoords, obstaclesYCoords);

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesLifes), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesXCoords), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesYCoords), 0);

}

TEST_F(ObstaclesRemove, none)
{

	populate(obstaclesLifes, { 1, 1, 1 });
	populateCoords(obstaclesXCoords, { 10.f, 20.f, 30.f });
	populateCoords(obstaclesYCoords, { 40.f, 50.f, 60.f });

	Game::Core::Obstacles::remove(obstaclesToRemove, obstaclesLifes, obstaclesXCoords, obstaclesYCoords);

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesLifes), 3);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesXCoords), 3);
	ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesYCoords), 3);

	EXPECT_EQ(Dod::BufferUtils::get(obstaclesXCoords, 0), 10.f);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesXCoords, 1), 20.f);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesXCoords, 2), 30.f);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesYCoords, 0), 40.f);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesYCoords, 1), 50.f);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesYCoords, 2), 60.f);

}

TEST_F(ObstaclesRemove, inverted)
{

	populate(obstaclesLifes, { 1, 1, 1 });
	populate(obstaclesToRemove, { 2, 1, 0 });
	populateCoords(obstaclesXCoords, { 10.f, 20.f, 30.f });
	populateCoords(obstaclesYCoords, { 40.f, 50.f, 60.f });

	Game::Core::Obstacles::remove(obstaclesToRemove, obstaclesLifes, obstaclesXCoords, obstaclesYCoords);

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesLifes), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesXCoords), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesYCoords), 0);

}

TEST_F(ObstaclesRemove, partial)
{

	populate(obstaclesLifes, { 1, 1, 1 });
	populate(obstaclesToRemove, { 1 });
	populateCoords(obstaclesXCoords, { 10.f, 20.f, 30.f });
	populateCoords(obstaclesYCoords, { 40.f, 50.f, 60.f });

	Game::Core::Obstacles::remove(obstaclesToRemove, obstaclesLifes, obstaclesXCoords, obstaclesYCoords);

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesLifes), 2);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesXCoords), 2);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesYCoords), 2);

	EXPECT_EQ(Dod::BufferUtils::get(obstaclesXCoords, 0), 10.f);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesXCoords, 1), 30.f);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesYCoords, 0), 40.f);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesYCoords, 1), 60.f);


	populate(obstaclesToRemove, { 1 });

	Game::Core::Obstacles::remove(obstaclesToRemove, obstaclesLifes, obstaclesXCoords, obstaclesYCoords);

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesLifes), 1);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesXCoords), 1);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesYCoords), 1);

	EXPECT_EQ(Dod::BufferUtils::get(obstaclesXCoords, 0), 10.f);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesYCoords, 0), 40.f);


	populate(obstaclesToRemove, { 0 });

	Game::Core::Obstacles::remove(obstaclesToRemove, obstaclesLifes, obstaclesXCoords, obstaclesYCoords);

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesLifes), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesXCoords), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesYCoords), 0);

}

TEST_F(ObstaclesRemove, partial2)
{

	populate(obstaclesLifes, { 1, 1, 1 });
	populate(obstaclesToRemove, { 0 });
	populateCoords(obstaclesXCoords, { 10.f, 20.f, 30.f });
	populateCoords(obstaclesYCoords, { 40.f, 50.f, 60.f });

	Game::Core::Obstacles::remove(obstaclesToRemove, obstaclesLifes, obstaclesXCoords, obstaclesYCoords);

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesLifes), 2);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesXCoords), 2);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesYCoords), 2);

	EXPECT_EQ(Dod::BufferUtils::get(obstaclesXCoords, 0), 30.f);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesXCoords, 1), 20.f);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesYCoords, 0), 60.f);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesYCoords, 1), 50.f);


	populate(obstaclesToRemove, { 1 });

	Game::Core::Obstacles::remove(obstaclesToRemove, obstaclesLifes, obstaclesXCoords, obstaclesYCoords);

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesLifes), 1);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesXCoords), 1);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesYCoords), 1);

	EXPECT_EQ(Dod::BufferUtils::get(obstaclesXCoords, 0), 30.f);
	EXPECT_EQ(Dod::BufferUtils::get(obstaclesYCoords, 0), 60.f);


	populate(obstaclesToRemove, { 0 });

	Game::Core::Obstacles::remove(obstaclesToRemove, obstaclesLifes, obstaclesXCoords, obstaclesYCoords);

	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesToRemove), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesLifes), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesXCoords), 0);
	EXPECT_EQ(Dod::BufferUtils::getNumFilledElements(obstaclesYCoords), 0);

}
