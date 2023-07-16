#include "Inputs.h"

#include <dod/ConditionTable.h>
#include <dod/BufferUtils.h>

#include <array>

template <size_t numOfCols>
using condInput_t = std::array<Dod::CondTable::TriState, numOfCols>;
template <size_t numOfRows, size_t numOfCols>
using condTableSrc_t = std::array<condInput_t<numOfCols>, numOfRows>;

int32_t Game::Inputs::computeFireComponent(uint32_t currentInputs, uint32_t prevInputs) noexcept
{

    // currPress, prevPress
    const condTableSrc_t<3, 2> tableSrc{ {
        { Dod::CondTable::TriState::TRUE, Dod::CondTable::TriState::FALSE },
        { Dod::CondTable::TriState::TRUE, Dod::CondTable::TriState::TRUE },
        { Dod::CondTable::TriState::FALSE, Dod::CondTable::TriState::TRUE },
    } };

    std::array<uint32_t, tableSrc.size() + 1> xOrMasksMem;
    std::array<uint32_t, tableSrc.size() + 1> ignoreMem;

    const auto table{ Dod::CondTable::generate(tableSrc, xOrMasksMem, ignoreMem) };

    const uint32_t inputs{
        (uint32_t((currentInputs & 4) != 0) << 0) |
        (uint32_t((prevInputs & 4) != 0) << 1)
    };

    std::array<int32_t, 16> qValuesMem;
    Dod::DBBuffer<int32_t> qValues;
    Dod::BufferUtils::initFromArray(qValues, qValuesMem);

    Dod::CondTable::populateQuery(qValues, inputs, table);

    const auto transformOutputs{ std::to_array<int32_t>({
        1,
        0,
        0,
    }) };

    int32_t fireComponent{};
    Dod::CondTable::applyTransform<int32_t, int32_t>(fireComponent, transformOutputs, Dod::BufferUtils::createImFromBuffer(qValues));

    return fireComponent;

}

float Game::Inputs::computeMoveComponent(uint32_t currentInputs, uint32_t prevInputs, float prevMovement) noexcept
{

    // currLeft, prevLeft, currRight, prevRight
    const condTableSrc_t<4, 4> tableSrc{ {
        { Dod::CondTable::TriState::TRUE, Dod::CondTable::TriState::FALSE, Dod::CondTable::TriState::SKIP, Dod::CondTable::TriState::SKIP },
        { Dod::CondTable::TriState::SKIP, Dod::CondTable::TriState::SKIP, Dod::CondTable::TriState::TRUE, Dod::CondTable::TriState::FALSE },
        { Dod::CondTable::TriState::FALSE, Dod::CondTable::TriState::TRUE, Dod::CondTable::TriState::FALSE, Dod::CondTable::TriState::SKIP },
        { Dod::CondTable::TriState::FALSE, Dod::CondTable::TriState::SKIP, Dod::CondTable::TriState::FALSE, Dod::CondTable::TriState::TRUE },
    } };

    std::array<uint32_t, tableSrc.size() + 1> xOrMasksMem;
    std::array<uint32_t, tableSrc.size() + 1> ignoreMem;

    const auto table{ Dod::CondTable::generate(tableSrc, xOrMasksMem, ignoreMem) };

    const uint32_t inputs{
        (uint32_t((currentInputs & 1) != 0) << 0) |
        (uint32_t((prevInputs & 1) != 0) << 1) |
        (uint32_t((currentInputs & 2) != 0) << 2) |
        (uint32_t((prevInputs & 2) != 0) << 3)
    };

    std::array<int32_t, 16> qValuesMem;
    Dod::DBBuffer<int32_t> qValues;
    Dod::BufferUtils::initFromArray(qValues, qValuesMem);

    Dod::CondTable::populateQuery(qValues, inputs, table);

    const auto transformOutputs{ std::to_array<float>({
        -1.f,
        1.f,
        0.f,
        0.f,
    }) };

    float moveComponent{ prevMovement };
    Dod::CondTable::applyTransform<int32_t, float>(moveComponent, transformOutputs, Dod::BufferUtils::createImFromBuffer(qValues));

    return moveComponent;

}
