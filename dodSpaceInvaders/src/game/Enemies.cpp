#include "Enemies.h"

#include <dod/ConditionTable.h>

#include <array>

template <size_t numOfCols>
using condInput_t = std::array<Dod::CondTable::TriState, numOfCols>;
template <size_t numOfRows, size_t numOfCols>
using condTableSrc_t = std::array<condInput_t<numOfCols>, numOfRows>;

bool Game::Enemies::computeNeedChangeDirection(float currentDirection, float currentXPosition, float mostLeft, float mostRight) noexcept
{

    // xOnLeftSide, xOnRightSide, directionLeft, directionRight
    const condTableSrc_t<2, 4> tableSrc{ {
        { Dod::CondTable::TriState::TRUE, Dod::CondTable::TriState::SKIP, Dod::CondTable::TriState::TRUE, Dod::CondTable::TriState::SKIP },
        { Dod::CondTable::TriState::SKIP, Dod::CondTable::TriState::TRUE, Dod::CondTable::TriState::SKIP, Dod::CondTable::TriState::TRUE },
    } };

    std::array<uint32_t, tableSrc.size() + 1> xOrMasksMem;
    std::array<uint32_t, tableSrc.size() + 1> ignoreMem;

    const auto table{ Dod::CondTable::generate(tableSrc, xOrMasksMem, ignoreMem) };


    constexpr uint32_t xOnTheLeft{ 1 << 0 };
    constexpr uint32_t xOnTheRight{ 1 << 1 };
    constexpr uint32_t directionLeft{ 1 << 2 };
    constexpr uint32_t directionRight{ 1 << 3 };
    const auto generateInput = [](float currentDirection, float currentXPosition) -> uint32_t {
        uint32_t bits{};
        bits |= (xOnTheLeft) * (currentXPosition < 75.f);
        bits |= (xOnTheRight) * (currentXPosition > 275.f);
        bits |= (directionLeft) * (currentDirection <= -1.f);
        bits |= (directionRight) * (currentDirection >= 1.f);
        return bits;
    };
    const auto inputs{ generateInput(currentDirection, currentXPosition) };

    std::array<int32_t, 16> qValuesMem;
    Dod::DBBuffer<int32_t> qValues;
    Dod::BufferUtils::initFromArray(qValues, qValuesMem);
    Dod::CondTable::populateQuery(qValues, inputs, table);

    const auto transformOutputs{ std::to_array<bool>({
        true,
        true,
    }) };

    bool bNeedChangeDirection{ false };
    Dod::CondTable::applyTransform<int32_t, bool>(bNeedChangeDirection, transformOutputs, Dod::BufferUtils::createImFromBuffer(qValues));

    return bNeedChangeDirection;

}

float Game::Enemies::computeNewDirection(float currentDirection, bool strobe) noexcept
{

    const auto newDirection{ currentDirection + (-currentDirection) * 2.f * strobe };

    return newDirection;

}

float Game::Enemies::computeNewYPosition(float currentYPosition, float stride, bool strobe) noexcept
{

    const auto newBatchYCoord{ currentYPosition + stride * strobe };

    return newBatchYCoord;

}

float Game::Enemies::computeNewXPosition(float currentDirection, float currentXPosition, float stride, bool strobe) noexcept
{

    const auto newXPosition{ currentXPosition + stride * currentDirection * strobe };

    return newXPosition;

}

float Game::Enemies::updateStrobeCountdown(float dt, float prevTimeLeft, float initialTime) noexcept
{
    const auto newValue{ prevTimeLeft - dt };
    const auto bReset{ newValue <= 0 };
    return newValue + bReset * (initialTime - newValue);
}

bool Game::Enemies::updateStrobe(float prevTimeLeft, float currentTime) noexcept
{
    return currentTime > prevTimeLeft;
}

void Game::Enemies::generateX(
    Dod::DBBuffer<float>& xCoords,
    int32_t elementsPerRow,
    int32_t elementsPerCol,
    float offset,
    float stride
) noexcept
{
    const auto totalElements{ elementsPerCol * elementsPerRow };

    for (int32_t enemyId{ 0 }; enemyId < totalElements; ++enemyId)
    {
        const auto inRowId{ enemyId % elementsPerRow };
        const auto xPosition{ offset + static_cast<float>(inRowId) * stride };
        Dod::BufferUtils::populate(xCoords, xPosition, true);
    }
}

void Game::Enemies::generateY(
    Dod::DBBuffer<float>& yCoords,
    int32_t elementsPerRow,
    int32_t elementsPerCol,
    float offset,
    float stride
) noexcept
{
    const auto totalElements{ elementsPerCol * elementsPerRow };

    for (int32_t enemyId{ 0 }; enemyId < totalElements; ++enemyId)
    {
        const auto inColId{ enemyId / elementsPerRow };
        const auto yPosition{ offset + static_cast<float>(inColId) * stride };
        Dod::BufferUtils::populate(yCoords, yPosition, true);
    }
}
