#include "pch.h"

#include <game/Enemies.cpp>
#include <dod/BufferUtils.h>

#include <array>

TEST(Enemies, NeedUpdateDirection)
{

	struct Input 
	{
		const float currentDirection{ };
		const float currentXPosition{ };
		const float mostLeft{ };
		const float mostRight{ };
	};

	const auto inputs{ std::to_array<Input>({
		{1.f, 280.f, 75.f, 275.f},
		{1.f, 276.f, 75.f, 275.f},

		{1.f, 275.f, 75.f, 275.f},

		{-1.f, 280.f, 75.f, 275.f},
		{-1.f, 275.f, 75.f, 275.f},

		{0.f, 280.f, 75.f, 275.f},
		{0.f, 275.f, 75.f, 275.f},

		{1.f, 100.f, 75.f, 275.f},

		{0.f, 75.f, 75.f, 275.f},
		{0.f, 74.f, 75.f, 275.f},

		{1.f, 75.f, 75.f, 275.f},
		{1.f, 74.f, 75.f, 275.f},

		{-1.f, 75.f, 75.f, 275.f},

		{-1.f, 74.f, 75.f, 275.f},
		{-1.f, 60.f, 75.f, 275.f},
	}) };

	const auto expected{ std::to_array<bool>({
		true,
		true,

		false,

		false,
		false,

		false,
		false,

		false,

		false,
		false,

		false,
		false,

		false,

		true,
		true,
	}) };

	static_assert(inputs.size() == expected.size());

	for (size_t elId{}; elId < inputs.size(); ++elId)
	{
		const auto bNeedChangeDirection{ Game::Enemies::computeNeedChangeDirection(
			inputs[elId].currentDirection, 
			inputs[elId].currentXPosition, 
			inputs[elId].mostLeft, 
			inputs[elId].mostRight) };
		EXPECT_EQ(bNeedChangeDirection, expected[elId]);
	}

}

TEST(Enemies, NewDirection)
{

	{
		const auto bNeedUpdateDirection{ true };
		const auto currentDirection{ 1.f };
		EXPECT_FLOAT_EQ(Game::Enemies::computeNewDirection(currentDirection, bNeedUpdateDirection), -1.f);
	}
	{
		const auto bNeedUpdateDirection{ true };
		const auto currentDirection{ -1.f };
		EXPECT_FLOAT_EQ(Game::Enemies::computeNewDirection(currentDirection, bNeedUpdateDirection), 1.f);
	}
	{
		const auto bNeedUpdateDirection{ false };
		const auto currentDirection{ 1.f };
		EXPECT_FLOAT_EQ(Game::Enemies::computeNewDirection(currentDirection, bNeedUpdateDirection), 1.f);
	}
	{
		const auto bNeedUpdateDirection{ false };
		const auto currentDirection{ -1.f };
		EXPECT_FLOAT_EQ(Game::Enemies::computeNewDirection(currentDirection, bNeedUpdateDirection), -1.f);
	}

}

TEST(Enemies, NewPositionY)
{

	{
		const auto bNeedUpdatePosition{ true };
		const auto currentYPosition{ 100.f };
		const auto stride{ 25.f };
		EXPECT_FLOAT_EQ(Game::Enemies::computeNewYPosition(currentYPosition, stride, bNeedUpdatePosition), 125.f);
	}
	{
		const auto bNeedUpdatePosition{ true };
		const auto currentYPosition{ 100.f };
		const auto stride{ 75.f };
		EXPECT_FLOAT_EQ(Game::Enemies::computeNewYPosition(currentYPosition, stride, bNeedUpdatePosition), 175.f);
	}
	{
		const auto bNeedUpdatePosition{ true };
		const auto currentYPosition{ 200.f };
		const auto stride{ 25.f };
		EXPECT_FLOAT_EQ(Game::Enemies::computeNewYPosition(currentYPosition, stride, bNeedUpdatePosition), 225.f);
	}
	{
		const auto bNeedUpdatePosition{ false };
		const auto currentYPosition{ 100.f };
		const auto stride{ 25.f };
		EXPECT_FLOAT_EQ(Game::Enemies::computeNewYPosition(currentYPosition, stride, bNeedUpdatePosition), 100.f);
	}
	{
		const auto bNeedUpdatePosition{ false };
		const auto currentYPosition{ 100.f };
		const auto stride{ 75.f };
		EXPECT_FLOAT_EQ(Game::Enemies::computeNewYPosition(currentYPosition, stride, bNeedUpdatePosition), 100.f);
	}
	{
		const auto bNeedUpdatePosition{ false };
		const auto currentYPosition{ 200.f };
		const auto stride{ 25.f };
		EXPECT_FLOAT_EQ(Game::Enemies::computeNewYPosition(currentYPosition, stride, bNeedUpdatePosition), 200.f);
	}

}

TEST(Enemies, NewPositionX)
{

	struct Input
	{
		const float currentDirection{};
		const float currentXPosition{};
		const float stride{};
		const bool needUpdate{};
	};

	const auto inputs{ std::to_array<Input>({
		{1.f, 70.f, 25.f, true},
		{1.f, 170.f, 25.f, true},
		{1.f, 70.f, 120.f, true},
		{1.f, 70.f, 25.f, false},
		{-1.f, 70.f, 25.f, true},
		{-1.f, 170.f, 25.f, true},
		{-1.f, 70.f, 120.f, true},
		{-1.f, 70.f, 25.f, false},
	}) };

	const auto expected{ std::to_array<float>({
		95.f,
		195.f,
		190.f,
		70.f,
		45.f,
		145.f,
		-50.f,
		70.f,
	}) };

	static_assert(inputs.size() == expected.size());

	for (size_t elId{}; elId < inputs.size(); ++elId)
	{
		const auto newPositionX{ Game::Enemies::computeNewXPosition(
			inputs[elId].currentDirection,
			inputs[elId].currentXPosition,
			inputs[elId].stride,
			inputs[elId].needUpdate) };
		EXPECT_EQ(newPositionX, expected[elId]);
	}

}

TEST(Enemies, UpdateStrobeCountdown)
{

	{
		const auto prevTime{ 0.5f };
		const auto deltaTime{ 0.1f };
		const auto initialTime{ 1.f };
		EXPECT_FLOAT_EQ(Game::Enemies::updateStrobeCountdown(deltaTime, prevTime, initialTime), 0.4f);
	}
	{
		const auto prevTime{ 0.2f };
		const auto deltaTime{ 0.1f };
		const auto initialTime{ 1.f };
		EXPECT_FLOAT_EQ(Game::Enemies::updateStrobeCountdown(deltaTime, prevTime, initialTime), 0.1f);
	}
	{
		const auto prevTime{ 0.6f };
		const auto deltaTime{ 0.1f };
		const auto initialTime{ 1.f };
		EXPECT_FLOAT_EQ(Game::Enemies::updateStrobeCountdown(deltaTime, prevTime, initialTime), 0.5f);
	}
	{
		const auto prevTime{ 0.1f };
		const auto deltaTime{ 0.1f };
		const auto initialTime{ 1.f };
		EXPECT_FLOAT_EQ(Game::Enemies::updateStrobeCountdown(deltaTime, prevTime, initialTime), 1.0f);
	}
	{
		const auto prevTime{ 0.1f };
		const auto deltaTime{ 0.2f };
		const auto initialTime{ 1.f };
		EXPECT_FLOAT_EQ(Game::Enemies::updateStrobeCountdown(deltaTime, prevTime, initialTime), 1.0f);
	}
	{
		const auto prevTime{ 0.1f };
		const auto deltaTime{ 0.1f };
		const auto initialTime{ 2.f };
		EXPECT_FLOAT_EQ(Game::Enemies::updateStrobeCountdown(deltaTime, prevTime, initialTime), 2.0f);
	}
	{
		const auto prevTime{ 0.1f };
		const auto deltaTime{ 0.2f };
		const auto initialTime{ 2.f };
		EXPECT_FLOAT_EQ(Game::Enemies::updateStrobeCountdown(deltaTime, prevTime, initialTime), 2.0f);
	}

}

TEST(Enemies, UpdateStrobe)
{
	{
		const auto prevTime{ 0.1f };
		const auto currentTime{ 1.f };
		EXPECT_TRUE(Game::Enemies::updateStrobe(prevTime, currentTime));
	}
	{
		const auto prevTime{ 0.5f };
		const auto currentTime{ 0.2f };
		EXPECT_FALSE(Game::Enemies::updateStrobe(prevTime, currentTime));
	}
	{
		const auto prevTime{ 0.5f };
		const auto currentTime{ -0.1f };
		EXPECT_FALSE(Game::Enemies::updateStrobe(prevTime, currentTime));
	}
	{
		const auto prevTime{ 1.f };
		const auto currentTime{ 1.f };
		EXPECT_FALSE(Game::Enemies::updateStrobe(prevTime, currentTime));
	}
}

TEST(Enemies, Generation)
{

	const auto enemiesPerRow{ 5 };
	const auto enemiesPerCol{ 3 };
	const auto totalEnemies{ enemiesPerRow * enemiesPerCol };

	std::array<float, totalEnemies + 1> xCoordsMem;
	std::array<float, totalEnemies + 1> yCoordsMem;

	Dod::DBBuffer<float> xCoords;
	Dod::BufferUtils::initFromArray(xCoords, xCoordsMem);
	Dod::DBBuffer<float> yCoords;
	Dod::BufferUtils::initFromArray(yCoords, yCoordsMem);

	const auto strideX{ 25.f };
	const auto strideY{ 15.f };

	const auto offsetX{ 10.f };
	const auto offsetY{ 20.f };

	Game::Enemies::generateX(xCoords, enemiesPerRow, enemiesPerCol, offsetX, strideX);
	Game::Enemies::generateY(yCoords, enemiesPerRow, enemiesPerCol, offsetY, strideY);

	ASSERT_EQ(xCoords.numOfFilledEls, totalEnemies);
	ASSERT_EQ(yCoords.numOfFilledEls, totalEnemies);

	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 0), offsetX + strideX * 0);
	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 1), offsetX + strideX * 1);
	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 2), offsetX + strideX * 2);
	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 3), offsetX + strideX * 3);
	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 4), offsetX + strideX * 4);

	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 5), offsetX + strideX * 0);
	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 6), offsetX + strideX * 1);
	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 7), offsetX + strideX * 2);
	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 8), offsetX + strideX * 3);
	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 9), offsetX + strideX * 4);

	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 10), offsetX + strideX * 0);
	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 11), offsetX + strideX * 1);
	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 12), offsetX + strideX * 2);
	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 13), offsetX + strideX * 3);
	EXPECT_EQ(Dod::BufferUtils::get(xCoords, 14), offsetX + strideX * 4);


	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 0), offsetY + strideY * 0);
	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 1), offsetY + strideY * 0);
	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 2), offsetY + strideY * 0);
	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 3), offsetY + strideY * 0);
	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 4), offsetY + strideY * 0);

	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 5), offsetY + strideY * 1);
	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 6), offsetY + strideY * 1);
	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 7), offsetY + strideY * 1);
	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 8), offsetY + strideY * 1);
	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 9), offsetY + strideY * 1);

	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 10), offsetY + strideY * 2);
	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 11), offsetY + strideY * 2);
	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 12), offsetY + strideY * 2);
	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 13), offsetY + strideY * 2);
	EXPECT_EQ(Dod::BufferUtils::get(yCoords, 14), offsetY + strideY * 2);

}

TEST(Enemies, BulletsGeneration)
{

	constexpr auto totalSources{ 5 };
	struct RandomGen1
	{
		std::array<int32_t, totalSources> values{ 1, 2, 3, 4, 5 };
		size_t currentValue{};
		[[nodiscrad]] const auto generate(int32_t minimum, int32_t maximum) noexcept
		{
			return values[this->currentValue++];
		}
	};

	std::array<float, totalSources + 1> spawnPositionsMemP{ 0.f, 10.f, 20.f, 30.f, 40.f, 50.f };
	Dod::DBBuffer<float> spawnPositions;
	Dod::BufferUtils::initFromArray(spawnPositions, spawnPositionsMemP);
	spawnPositions.numOfFilledEls = totalSources;

	{
		const auto strobe{ true };
		RandomGen1 gen;
		std::array<float, totalSources + 1> positionsMem;
		Dod::DBBuffer<float> positions;
		Dod::BufferUtils::initFromArray(positions, positionsMem);

		{
			Game::Enemies::generateBullet(positions, spawnPositions, gen, strobe);
			ASSERT_EQ(positions.numOfFilledEls, 1);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 0), 10.f);
		}
		{
			Game::Enemies::generateBullet(positions, spawnPositions, gen, strobe);
			ASSERT_EQ(positions.numOfFilledEls, 2);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 0), 10.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 1), 20.f);
		}
		{
			Game::Enemies::generateBullet(positions, spawnPositions, gen, strobe);
			ASSERT_EQ(positions.numOfFilledEls, 3);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 0), 10.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 1), 20.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 2), 30.f);
		}
		{
			Game::Enemies::generateBullet(positions, spawnPositions, gen, strobe);
			ASSERT_EQ(positions.numOfFilledEls, 4);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 0), 10.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 1), 20.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 2), 30.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 3), 40.f);
		}
		{
			Game::Enemies::generateBullet(positions, spawnPositions, gen, strobe);
			ASSERT_EQ(positions.numOfFilledEls, 5);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 0), 10.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 1), 20.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 2), 30.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 3), 40.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 4), 50.f);
		}
	}

	{
		RandomGen1 gen;
		std::array<float, totalSources + 1> positionsMem;
		Dod::DBBuffer<float> positions;
		Dod::BufferUtils::initFromArray(positions, positionsMem);

		{
			const auto strobe{ true };
			Game::Enemies::generateBullet(positions, spawnPositions, gen, strobe);
			ASSERT_EQ(positions.numOfFilledEls, 1);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 0), 10.f);
		}
		{
			const auto strobe{ false };
			Game::Enemies::generateBullet(positions, spawnPositions, gen, strobe);
			ASSERT_EQ(positions.numOfFilledEls, 1);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 0), 10.f);
		}
		{
			const auto strobe{ true };
			Game::Enemies::generateBullet(positions, spawnPositions, gen, strobe);
			ASSERT_EQ(positions.numOfFilledEls, 2);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 0), 10.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 1), 30.f);
		}
		{
			const auto strobe{ true };
			Game::Enemies::generateBullet(positions, spawnPositions, gen, strobe);
			ASSERT_EQ(positions.numOfFilledEls, 3);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 0), 10.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 1), 30.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 2), 40.f);
		}
		{
			const auto strobe{ false };
			Game::Enemies::generateBullet(positions, spawnPositions, gen, strobe);
			ASSERT_EQ(positions.numOfFilledEls, 3);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 0), 10.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 1), 30.f);
			EXPECT_EQ(Dod::BufferUtils::get(positions, 2), 40.f);
		}
	}

}

TEST(Enemies, FireRule)
{

	constexpr auto totalTries{ 5 };
	struct RandomGen1
	{
		std::array<int32_t, totalTries> values{ 100, 300, 240, 900, 500 };
		size_t currentValue{};
		[[nodiscrad]] const auto generate(int32_t minimum, int32_t maximum) noexcept
		{
			return values[this->currentValue++];
		}
	};

	{
		RandomGen1 gen;
		{
			const auto strobe{ true };
			EXPECT_FALSE(Game::Enemies::fireRule(gen, strobe));
		}
		{
			const auto strobe{ true };
			EXPECT_TRUE(Game::Enemies::fireRule(gen, strobe));
		}
		{
			const auto strobe{ true };
			EXPECT_FALSE(Game::Enemies::fireRule(gen, strobe));
		}
		{
			const auto strobe{ true };
			EXPECT_TRUE(Game::Enemies::fireRule(gen, strobe));
		}
		{
			const auto strobe{ true };
			EXPECT_TRUE(Game::Enemies::fireRule(gen, strobe));
		}
	}
	{
		struct RandomGen2
		{
			[[nodiscrad]] const auto generate(int32_t minimum, int32_t maximum) noexcept
			{
				return 300;
			}
		};
		RandomGen2 gen;
		{
			const auto strobe{ false };
			EXPECT_FALSE(Game::Enemies::fireRule(gen, strobe));
		}
		{
			const auto strobe{ false };
			EXPECT_FALSE(Game::Enemies::fireRule(gen, strobe));
		}
		{
			const auto strobe{ false };
			EXPECT_FALSE(Game::Enemies::fireRule(gen, strobe));
		}
		{
			const auto strobe{ false };
			EXPECT_FALSE(Game::Enemies::fireRule(gen, strobe));
		}
		{
			const auto strobe{ false };
			EXPECT_FALSE(Game::Enemies::fireRule(gen, strobe));
		}
	}
	
}
