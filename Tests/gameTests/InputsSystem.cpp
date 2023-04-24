#include "pch.h"

#include <game/Inputs.cpp>

TEST(InputsSystem, Fire)
{

	struct InputData
	{
		const uint32_t currentInputs{};
		const uint32_t prevInputs{};
	};

	const auto inputs{ std::to_array<InputData>({
		InputData(1 << 2, 0),
		InputData(0xffff'ffff, 0),
		InputData(1 << 2, 1 << 2),
		InputData(0, 1 << 2),
		InputData(~(1 << 2), 0),
		InputData(~(1 << 2), 0xffff'ffff),
	}) };

	const auto expectedResults{ std::to_array<int32_t>({
		1,
		1,
		0, 
		0,
		0,
		0,
	}) };

	static_assert(inputs.size() == expectedResults.size());

	for (size_t id{ 0 }; id < inputs.size(); ++id)
	{
		const auto input{ inputs[id] };
		const auto fireComponent{ Game::Inputs::computeFireComponent(input.currentInputs, input.prevInputs) };

		EXPECT_EQ(fireComponent, expectedResults[id]);
	}

}

TEST(InputsSystem, Moving)
{

	struct InputData
	{
		const uint32_t currentInputs{};
		const uint32_t prevInputs{};
		const float prevMovement{};
	};

	const auto inputs{ std::to_array<InputData>({
		InputData(1 << 0, 0),
		InputData(1 << 0, 1 << 1),
		InputData((1 << 0) | (1 << 1), 1 << 1),
		InputData((1 << 0), (1 << 0) | (1 << 1), -1.f),

		InputData(1 << 1, 0),
		InputData(1 << 1, 1 << 0),
		InputData((1 << 1) | (1 << 0), 1 << 0),
		InputData((1 << 1), (1 << 0) | (1 << 1), 1.f),

		InputData((1 << 0), (1 << 0) | (1 << 1)),
		InputData((1 << 1), (1 << 0) | (1 << 1)),
		InputData(1 << 0, 1 << 0),
		InputData(1 << 1, 1 << 1),
		InputData(0, 1 << 1),
		InputData(0, 1 << 0),
	}) };

	const auto expectedResults{ std::to_array<float>({
		-1.f,
		-1.f,
		-1.f,
		-1.f,

		1.f,
		1.f,
		1.f,
		1.f,

		0.f,
		0.f,
		0.f,
		0.f,
		0.f,
		0.f,
	}) };

	static_assert(inputs.size() == expectedResults.size());

	for (size_t id{ 0 }; id < inputs.size(); ++id)
	{
		const auto input{ inputs[id] };
		const auto moveComponent{ Game::Inputs::computeMoveComponent(input.currentInputs, input.prevInputs, input.prevMovement) };

		EXPECT_EQ(moveComponent, expectedResults[id]);
	}

}
