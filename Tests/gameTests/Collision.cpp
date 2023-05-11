#include "pch.h"

#include <game/CollisionCore.cpp>

#include <dod/BufferUtils.h>

#include <array>
#include <vector>

class PointsAreaIntersection : public ::testing::Test {

protected:
    static constexpr int32_t maxElements{ 3 };
    static constexpr int32_t maxCollisions{ maxElements * maxElements };

    void SetUp() override
    {

        Dod::BufferUtils::initFromArray(xCoordLeft, xCoordLeftMem);
        Dod::BufferUtils::initFromArray(xAxisCollided, xAxisCollidedMem);

    }

    void initiatePoints(auto&& target, std::vector<float>&& data)
    {
        for (const auto el : data)
            Dod::BufferUtils::populate(target, el, true);
    }

    std::array<float, maxElements + 1> xCoordLeftMem{};
    std::array<int32_t, maxCollisions + 1> xAxisCollidedMem{};

    Dod::DBBuffer<float> xCoordLeft;
    Dod::DBBuffer<int32_t> xAxisCollided;

};

TEST_F(PointsAreaIntersection, inCenter)
{

    initiatePoints(xCoordLeft, { 0.f, 0.f, 0.f });

    constexpr auto maxRadius{ 2 };
    constexpr auto areaCenterX{ 0.f };

    Game::Core::Collision::pointsAreaIntersection(
        xAxisCollided,
        Dod::BufferUtils::createImFromBuffer(xCoordLeft),
        areaCenterX,
        maxRadius
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(xAxisCollided), 3);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 0), 0);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 1), 1);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 2), 2);

}

TEST_F(PointsAreaIntersection, areaOffset)
{

    initiatePoints(xCoordLeft, { 0.f, 0.f, 0.f });

    constexpr auto maxRadius{ 2 };
    constexpr auto areaCenterX{ 0.5f };

    Game::Core::Collision::pointsAreaIntersection(
        xAxisCollided,
        Dod::BufferUtils::createImFromBuffer(xCoordLeft),
        areaCenterX,
        maxRadius
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(xAxisCollided), 3);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 0), 0);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 1), 1);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 2), 2);

}

TEST_F(PointsAreaIntersection, areaOffsetInverted)
{

    initiatePoints(xCoordLeft, { 0.f, 0.f, 0.f });

    constexpr auto maxRadius{ 2 };
    constexpr auto areaCenterX{ -0.5f };

    Game::Core::Collision::pointsAreaIntersection(
        xAxisCollided,
        Dod::BufferUtils::createImFromBuffer(xCoordLeft),
        areaCenterX,
        maxRadius
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(xAxisCollided), 3);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 0), 0);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 1), 1);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 2), 2);

}

TEST_F(PointsAreaIntersection, areaOffset2)
{

    initiatePoints(xCoordLeft, { 0.f, 0.f, 0.f });

    constexpr auto maxRadius{ 2 };
    constexpr auto areaCenterX{ 2.5f };

    Game::Core::Collision::pointsAreaIntersection(
        xAxisCollided,
        Dod::BufferUtils::createImFromBuffer(xCoordLeft),
        areaCenterX,
        maxRadius
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(xAxisCollided), 0);

}

TEST_F(PointsAreaIntersection, areaOffset2Inverted)
{

    initiatePoints(xCoordLeft, { 0.f, 0.f, 0.f });

    constexpr auto maxRadius{ 2 };
    constexpr auto areaCenterX{ -2.5f };

    Game::Core::Collision::pointsAreaIntersection(
        xAxisCollided,
        Dod::BufferUtils::createImFromBuffer(xCoordLeft),
        areaCenterX,
        maxRadius
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(xAxisCollided), 0);

}

TEST_F(PointsAreaIntersection, pointsOffset)
{

    initiatePoints(xCoordLeft, { -2.5f, 0.5f, 3.5f });

    constexpr auto maxRadius{ 2 };
    constexpr auto areaCenterX{ 0.f };

    Game::Core::Collision::pointsAreaIntersection(
        xAxisCollided,
        Dod::BufferUtils::createImFromBuffer(xCoordLeft),
        areaCenterX,
        maxRadius
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(xAxisCollided), 1);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 0), 1);

}

TEST_F(PointsAreaIntersection, pointsOffsetRadius)
{

    initiatePoints(xCoordLeft, { -2.5f, 0.5f, 3.5f });

    constexpr auto maxRadius{ 3 };
    constexpr auto areaCenterX{ 0.f };

    Game::Core::Collision::pointsAreaIntersection(
        xAxisCollided,
        Dod::BufferUtils::createImFromBuffer(xCoordLeft),
        areaCenterX,
        maxRadius
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(xAxisCollided), 2);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 0), 0);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 1), 1);

}

class PointsAreasIntersection : public ::testing::Test {

protected:
    static constexpr int32_t maxElements{ 3 };
    static constexpr int32_t maxCollisions{ maxElements * maxElements };

    void SetUp() override
    {

        Dod::BufferUtils::initFromArray(xCoordLeft, xCoordLeftMem);
        Dod::BufferUtils::initFromArray(xCoordRight, xCoordRightMem);
        Dod::BufferUtils::initFromArray(xAxisCollided, xAxisCollidedMem);

    }

    void initiatePoints(auto&& target, std::vector<float>&& data)
    {
        for (const auto el : data)
            Dod::BufferUtils::populate(target, el, true);
    }

    std::array<float, maxElements + 1> xCoordLeftMem{};
    std::array<float, maxElements + 1> xCoordRightMem{};
    std::array<uint64_t, maxCollisions + 1> xAxisCollidedMem{};

    Dod::DBBuffer<float> xCoordLeft;
    Dod::DBBuffer<float> xCoordRight;
    Dod::DBBuffer<uint64_t> xAxisCollided;

};

TEST_F(PointsAreasIntersection, inCenter)
{

    initiatePoints(xCoordLeft, { 0.f, 0.f, 0.f });
    initiatePoints(xCoordRight, { 0.f, 0.f, 0.f });

    constexpr auto maxRadius{ 2 };

    Game::Core::Collision::pointsAreasIntersection(
        xAxisCollided,
        Dod::BufferUtils::createImFromBuffer(xCoordLeft),
        Dod::BufferUtils::createImFromBuffer(xCoordRight),
        maxRadius
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(xAxisCollided), 9);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 0), (static_cast<int64_t>(0) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 1), (static_cast<int64_t>(0) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 2), (static_cast<int64_t>(0) << 32) | (2 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 3), (static_cast<int64_t>(1) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 4), (static_cast<int64_t>(1) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 5), (static_cast<int64_t>(1) << 32) | (2 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 6), (static_cast<int64_t>(2) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 7), (static_cast<int64_t>(2) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 8), (static_cast<int64_t>(2) << 32) | (2 << 0));

}

TEST_F(PointsAreasIntersection, leftMoved)
{

    initiatePoints(xCoordLeft, { 0.5f, 2.f, 2.5f });
    initiatePoints(xCoordRight, { 0.f, 0.f, 0.f });

    constexpr auto maxRadius{ 2 };

    Game::Core::Collision::pointsAreasIntersection(
        xAxisCollided,
        Dod::BufferUtils::createImFromBuffer(xCoordLeft),
        Dod::BufferUtils::createImFromBuffer(xCoordRight),
        maxRadius
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(xAxisCollided), 6);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 0), (static_cast<int64_t>(0) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 1), (static_cast<int64_t>(0) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 2), (static_cast<int64_t>(0) << 32) | (2 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 3), (static_cast<int64_t>(1) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 4), (static_cast<int64_t>(1) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 5), (static_cast<int64_t>(1) << 32) | (2 << 0));

}

TEST_F(PointsAreasIntersection, leftInverted)
{

    initiatePoints(xCoordLeft, { -0.5f, -2.f, -2.5f });
    initiatePoints(xCoordRight, { 0.f, 0.f, 0.f });

    constexpr auto maxRadius{ 2 };

    Game::Core::Collision::pointsAreasIntersection(
        xAxisCollided,
        Dod::BufferUtils::createImFromBuffer(xCoordLeft),
        Dod::BufferUtils::createImFromBuffer(xCoordRight),
        maxRadius
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(xAxisCollided), 6);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 0), (static_cast<int64_t>(0) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 1), (static_cast<int64_t>(0) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 2), (static_cast<int64_t>(0) << 32) | (2 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 3), (static_cast<int64_t>(1) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 4), (static_cast<int64_t>(1) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 5), (static_cast<int64_t>(1) << 32) | (2 << 0));

}

TEST_F(PointsAreasIntersection, rightMoved)
{

    initiatePoints(xCoordLeft, { 0.f, 0.f, 0.f });
    initiatePoints(xCoordRight, { 0.5f, 2.f, 2.5f });

    constexpr auto maxRadius{ 2 };

    Game::Core::Collision::pointsAreasIntersection(
        xAxisCollided,
        Dod::BufferUtils::createImFromBuffer(xCoordLeft),
        Dod::BufferUtils::createImFromBuffer(xCoordRight),
        maxRadius
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(xAxisCollided), 6);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 0), (static_cast<int64_t>(0) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 1), (static_cast<int64_t>(0) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 2), (static_cast<int64_t>(1) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 3), (static_cast<int64_t>(1) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 4), (static_cast<int64_t>(2) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 5), (static_cast<int64_t>(2) << 32) | (1 << 0));

}

TEST_F(PointsAreasIntersection, rightInverted)
{

    initiatePoints(xCoordLeft, { 0.f, 0.f, 0.f });
    initiatePoints(xCoordRight, { -0.5f, -2.f, -2.5f });

    constexpr auto maxRadius{ 2 };

    Game::Core::Collision::pointsAreasIntersection(
        xAxisCollided,
        Dod::BufferUtils::createImFromBuffer(xCoordLeft),
        Dod::BufferUtils::createImFromBuffer(xCoordRight),
        maxRadius
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(xAxisCollided), 6);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 0), (static_cast<int64_t>(0) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 1), (static_cast<int64_t>(0) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 2), (static_cast<int64_t>(1) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 3), (static_cast<int64_t>(1) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 4), (static_cast<int64_t>(2) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 5), (static_cast<int64_t>(2) << 32) | (1 << 0));

}

TEST_F(PointsAreasIntersection, lessRightPoints)
{

    initiatePoints(xCoordLeft, { 0.5f, 2.f, 2.5f });
    initiatePoints(xCoordRight, { 0.f, 0.f });

    constexpr auto maxRadius{ 2 };

    Game::Core::Collision::pointsAreasIntersection(
        xAxisCollided,
        Dod::BufferUtils::createImFromBuffer(xCoordLeft),
        Dod::BufferUtils::createImFromBuffer(xCoordRight),
        maxRadius
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(xAxisCollided), 4);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 0), (static_cast<int64_t>(0) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 1), (static_cast<int64_t>(0) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 2), (static_cast<int64_t>(1) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 3), (static_cast<int64_t>(1) << 32) | (1 << 0));

}

TEST_F(PointsAreasIntersection, lessRightPointsLargerRadius)
{

    initiatePoints(xCoordLeft, { 0.5f, 2.f, 2.4f });
    initiatePoints(xCoordRight, { 0.f, 0.f });

    constexpr auto maxRadius{ 3 };

    Game::Core::Collision::pointsAreasIntersection(
        xAxisCollided,
        Dod::BufferUtils::createImFromBuffer(xCoordLeft),
        Dod::BufferUtils::createImFromBuffer(xCoordRight),
        maxRadius
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(xAxisCollided), 6);
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 0), (static_cast<int64_t>(0) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 1), (static_cast<int64_t>(0) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 2), (static_cast<int64_t>(1) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 3), (static_cast<int64_t>(1) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 4), (static_cast<int64_t>(2) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(xAxisCollided, 5), (static_cast<int64_t>(2) << 32) | (1 << 0));

}

class PointsSquareIntersection : public ::testing::Test {

protected:
    static constexpr int32_t maxElements{ 3 };
    static constexpr int32_t maxCollisions{ maxElements };

    void SetUp() override
    {

        Dod::BufferUtils::initFromArray(this->xPointsCoords, this->xPointsCoordsMem);
        Dod::BufferUtils::initFromArray(this->yPointsCoords, this->yPointsCoordsMem);
        Dod::BufferUtils::initFromArray(intersections, intersectionsMem);

    }

    void initiatePoints(auto&& target, std::array<float, maxElements>&& data)
    {
        for (const auto el : data)
            Dod::BufferUtils::populate(target, el, true);
    }

    std::array<float, maxElements + 1> xPointsCoordsMem{};
    std::array<float, maxElements + 1> yPointsCoordsMem{};
    std::array<int32_t, maxCollisions + 1> intersectionsMem;

    Dod::DBBuffer<float> xPointsCoords;
    Dod::DBBuffer<float> yPointsCoords;
    Dod::DBBuffer<int32_t> intersections;

};

TEST_F(PointsSquareIntersection, inCenter)
{

    initiatePoints(xPointsCoords, { 0.f, 0.f, 0.f });
    initiatePoints(yPointsCoords, { 0.f, 0.f, 0.f });

    const auto squareX{ 0.f };
    const auto squareY{ 0.f };
    const auto squareWidth{ 2.f };
    const auto squareHeight{ 4.f };

    Game::Core::Collision::pointsSquareIntersection(
        intersections,
        Dod::BufferUtils::createImFromBuffer(xPointsCoords),
        Dod::BufferUtils::createImFromBuffer(yPointsCoords),
        squareX,
        squareY,
        squareWidth,
        squareHeight
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(intersections), 3);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 0), 0);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 1), 1);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 2), 2);

}

TEST_F(PointsSquareIntersection, offsetX)
{

    initiatePoints(xPointsCoords, { -2.5f, 0.5f, 1.f });
    initiatePoints(yPointsCoords, { 0.f, 0.f, 0.f });

    const auto squareX{ 0.f };
    const auto squareY{ 0.f };
    const auto squareWidth{ 2.f };
    const auto squareHeight{ 4.f };

    Game::Core::Collision::pointsSquareIntersection(
        intersections,
        Dod::BufferUtils::createImFromBuffer(xPointsCoords),
        Dod::BufferUtils::createImFromBuffer(yPointsCoords),
        squareX,
        squareY,
        squareWidth,
        squareHeight
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(intersections), 2);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 0), 1);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 1), 2);

}

TEST_F(PointsSquareIntersection, offsetY)
{

    initiatePoints(xPointsCoords, { 0.f, 0.f, 0.f });
    initiatePoints(yPointsCoords, { -2.f, 1.f, 5.f });

    const auto squareX{ 0.f };
    const auto squareY{ 0.f };
    const auto squareWidth{ 2.f };
    const auto squareHeight{ 4.f };

    Game::Core::Collision::pointsSquareIntersection(
        intersections,
        Dod::BufferUtils::createImFromBuffer(xPointsCoords),
        Dod::BufferUtils::createImFromBuffer(yPointsCoords),
        squareX,
        squareY,
        squareWidth,
        squareHeight
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(intersections), 2);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 0), 0);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 1), 1);

}

class PointsSquaresIntersection : public ::testing::Test {

protected:
    static constexpr int32_t maxElements{ 3 };
    static constexpr int32_t maxCollisions{ maxElements * maxElements };

    void SetUp() override 
    {

        Dod::BufferUtils::initFromArray(this->xPointsCoords, this->xPointsCoordsMem);
        Dod::BufferUtils::initFromArray(this->yPointsCoords, this->yPointsCoordsMem);
        Dod::BufferUtils::initFromArray(this->xSquareCoords, this->xSquareCoordsMem);
        Dod::BufferUtils::initFromArray(this->ySquareCoords, this->ySquareCoordsMem);
        Dod::BufferUtils::initFromArray(intersections, intersectionsMem);

    }

    void initiatePoints(auto&& target, std::array<float, maxElements>&& data)
    {
        for (const auto el : data)
            Dod::BufferUtils::populate(target, el, true);
    }

    std::array<float, maxElements + 1> xPointsCoordsMem{};
    std::array<float, maxElements + 1> yPointsCoordsMem{};
    std::array<float, maxElements + 1> xSquareCoordsMem{};
    std::array<float, maxElements + 1> ySquareCoordsMem{};
    std::array<uint64_t, maxCollisions + 1> intersectionsMem;

    Dod::DBBuffer<float> xPointsCoords;
    Dod::DBBuffer<float> yPointsCoords;
    Dod::DBBuffer<float> xSquareCoords;
    Dod::DBBuffer<float> ySquareCoords;
    Dod::DBBuffer<uint64_t> intersections;

};

TEST_F(PointsSquaresIntersection, inCenter)
{

    initiatePoints(xPointsCoords, { 0.f, 0.f, 0.f });
    initiatePoints(yPointsCoords, { 0.f, 0.f, 0.f });
    initiatePoints(xSquareCoords, { 0.f, 0.f, 0.f });
    initiatePoints(ySquareCoords, { 0.f, 0.f, 0.f });

    const auto squaresWidth{ 2.f };
    const auto squaresHeight{ 4.f };

    Game::Core::Collision::pointsSquaresIntersection(
        intersections,
        Dod::BufferUtils::createImFromBuffer(xPointsCoords),
        Dod::BufferUtils::createImFromBuffer(yPointsCoords),
        Dod::BufferUtils::createImFromBuffer(xSquareCoords),
        Dod::BufferUtils::createImFromBuffer(ySquareCoords),
        squaresWidth,
        squaresHeight
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(intersections), 9);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 0), (static_cast<int64_t>(0) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 1), (static_cast<int64_t>(0) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 2), (static_cast<int64_t>(0) << 32) | (2 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 3), (static_cast<int64_t>(1) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 4), (static_cast<int64_t>(1) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 5), (static_cast<int64_t>(1) << 32) | (2 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 6), (static_cast<int64_t>(2) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 7), (static_cast<int64_t>(2) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 8), (static_cast<int64_t>(2) << 32) | (2 << 0));

}

TEST_F(PointsSquaresIntersection, xOffset)
{

    initiatePoints(xPointsCoords, { -2.5f, 0.5f, 3.f });
    initiatePoints(yPointsCoords, { 0.f, 0.f, 0.f });
    initiatePoints(xSquareCoords, { 0.f, 0.f, 0.f });
    initiatePoints(ySquareCoords, { 0.f, 0.f, 0.f });

    const auto squaresWidth{ 2.f };
    const auto squaresHeight{ 4.f };

    Game::Core::Collision::pointsSquaresIntersection(
        intersections,
        Dod::BufferUtils::createImFromBuffer(xPointsCoords),
        Dod::BufferUtils::createImFromBuffer(yPointsCoords),
        Dod::BufferUtils::createImFromBuffer(xSquareCoords),
        Dod::BufferUtils::createImFromBuffer(ySquareCoords),
        squaresWidth,
        squaresHeight
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(intersections), 3);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 0), (static_cast<int64_t>(1) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 1), (static_cast<int64_t>(1) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 2), (static_cast<int64_t>(1) << 32) | (2 << 0));

}

TEST_F(PointsSquaresIntersection, yOffset)
{

    initiatePoints(xPointsCoords, { 0.f, 0.f, 0.f });
    initiatePoints(yPointsCoords, { -2.5f, 0.5f, 3.f });
    initiatePoints(xSquareCoords, { 0.f, 0.f, 0.f });
    initiatePoints(ySquareCoords, { 0.f, 0.f, 0.f });

    const auto squaresWidth{ 2.f };
    const auto squaresHeight{ 4.f };

    Game::Core::Collision::pointsSquaresIntersection(
        intersections,
        Dod::BufferUtils::createImFromBuffer(xPointsCoords),
        Dod::BufferUtils::createImFromBuffer(yPointsCoords),
        Dod::BufferUtils::createImFromBuffer(xSquareCoords),
        Dod::BufferUtils::createImFromBuffer(ySquareCoords),
        squaresWidth,
        squaresHeight
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(intersections), 9);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 0), (static_cast<int64_t>(0) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 1), (static_cast<int64_t>(0) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 2), (static_cast<int64_t>(0) << 32) | (2 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 3), (static_cast<int64_t>(1) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 4), (static_cast<int64_t>(1) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 5), (static_cast<int64_t>(1) << 32) | (2 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 6), (static_cast<int64_t>(2) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 7), (static_cast<int64_t>(2) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 8), (static_cast<int64_t>(2) << 32) | (2 << 0));

}

TEST_F(PointsSquaresIntersection, yOffsetShorter)
{

    initiatePoints(xPointsCoords, { 0.f, 0.f, 0.f });
    initiatePoints(yPointsCoords, { -2.5f, 0.5f, 3.f });
    initiatePoints(xSquareCoords, { 0.f, 0.f, 0.f });
    initiatePoints(ySquareCoords, { 0.f, 0.f, 0.f });

    const auto squaresWidth{ 2.f };
    const auto squaresHeight{ 2.5f };

    Game::Core::Collision::pointsSquaresIntersection(
        intersections,
        Dod::BufferUtils::createImFromBuffer(xPointsCoords),
        Dod::BufferUtils::createImFromBuffer(yPointsCoords),
        Dod::BufferUtils::createImFromBuffer(xSquareCoords),
        Dod::BufferUtils::createImFromBuffer(ySquareCoords),
        squaresWidth,
        squaresHeight
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(intersections), 6);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 0), (static_cast<int64_t>(0) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 1), (static_cast<int64_t>(0) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 2), (static_cast<int64_t>(0) << 32) | (2 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 3), (static_cast<int64_t>(1) << 32) | (0 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 4), (static_cast<int64_t>(1) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 5), (static_cast<int64_t>(1) << 32) | (2 << 0));

}

TEST_F(PointsSquaresIntersection, squaresOffsets)
{

    initiatePoints(xPointsCoords, { 0.f, 0.f, 0.f });
    initiatePoints(yPointsCoords, { 0.f, 0.f, 0.f });
    initiatePoints(xSquareCoords, { -2.5f, 0.5f, 0.1f });
    initiatePoints(ySquareCoords, { 2.5f, 1.f, -5.f });

    const auto squaresWidth{ 2.f };
    const auto squaresHeight{ 4.f };

    Game::Core::Collision::pointsSquaresIntersection(
        intersections,
        Dod::BufferUtils::createImFromBuffer(xPointsCoords),
        Dod::BufferUtils::createImFromBuffer(yPointsCoords),
        Dod::BufferUtils::createImFromBuffer(xSquareCoords),
        Dod::BufferUtils::createImFromBuffer(ySquareCoords),
        squaresWidth,
        squaresHeight
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(intersections), 3);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 0), (static_cast<int64_t>(0) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 1), (static_cast<int64_t>(1) << 32) | (1 << 0));
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 2), (static_cast<int64_t>(2) << 32) | (1 << 0));

}

class PointsPlaneCollision : public ::testing::Test {

protected:
    static constexpr int32_t maxElements{ 3 };
    static constexpr int32_t maxCollisions{ maxElements * maxElements };

    void SetUp() override
    {

        Dod::BufferUtils::initFromArray(this->yPointsCoords, this->yPointsCoordsMem);
        Dod::BufferUtils::initFromArray(intersections, intersectionsMem);

    }

    void populate(auto&& target, std::vector<float>&& data)
    {
        for (const auto el : data)
            Dod::BufferUtils::populate(target, el, true);
    }

    std::array<float, maxElements + 1> yPointsCoordsMem{};
    std::array<int32_t, maxCollisions + 1> intersectionsMem{};

    Dod::DBBuffer<float> yPointsCoords;
    Dod::DBBuffer<int32_t> intersections;

};

TEST_F(PointsPlaneCollision, initial)
{

    populate(yPointsCoords, { -1.f, 1.f, 2.f });

    const float planeY{ 0.f };
    const float direction{ 1.f };

    Game::Core::Collision::pointsPlaneIntersection(
        intersections, 
        Dod::BufferUtils::createImFromBuffer(yPointsCoords),
        planeY, 
        direction
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(intersections), 1);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 0), 0);

}

TEST_F(PointsPlaneCollision, pointsOffset)
{

    populate(yPointsCoords, { -2.f, 0.f, 1.f });

    const float planeY{ 0.f };
    const float direction{ 1.f };

    Game::Core::Collision::pointsPlaneIntersection(
        intersections,
        Dod::BufferUtils::createImFromBuffer(yPointsCoords),
        planeY,
        direction
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(intersections), 2);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 0), 0);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 1), 1);

}

TEST_F(PointsPlaneCollision, planeOffset)
{

    populate(yPointsCoords, { -1.f, 1.f, 2.f });

    const float planeY{ 1.5f };
    const float direction{ 1.f };

    Game::Core::Collision::pointsPlaneIntersection(
        intersections,
        Dod::BufferUtils::createImFromBuffer(yPointsCoords),
        planeY,
        direction
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(intersections), 2);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 0), 0);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 1), 1);

}

TEST_F(PointsPlaneCollision, inverted)
{

    populate(yPointsCoords, { -1.f, 1.f, 2.f });

    const float planeY{ 0.f };
    const float direction{ -1.f };

    Game::Core::Collision::pointsPlaneIntersection(
        intersections,
        Dod::BufferUtils::createImFromBuffer(yPointsCoords),
        planeY,
        direction
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(intersections), 2);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 0), 1);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 1), 2);

}

TEST_F(PointsPlaneCollision, pointsOffsetInverted)
{

    populate(yPointsCoords, { -2.f, 0.f, 1.f });

    const float planeY{ 0.f };
    const float direction{ -1.f };

    Game::Core::Collision::pointsPlaneIntersection(
        intersections,
        Dod::BufferUtils::createImFromBuffer(yPointsCoords),
        planeY,
        direction
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(intersections), 2);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 0), 1);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 1), 2);

}

TEST_F(PointsPlaneCollision, planeOffsetInverted)
{

    populate(yPointsCoords, { -1.f, 1.f, 2.f });

    const float planeY{ 1.5f };
    const float direction{ -1.f };

    Game::Core::Collision::pointsPlaneIntersection(
        intersections,
        Dod::BufferUtils::createImFromBuffer(yPointsCoords),
        planeY,
        direction
    );

    ASSERT_EQ(Dod::BufferUtils::getNumFilledElements(intersections), 1);
    EXPECT_EQ(Dod::BufferUtils::get(intersections, 0), 2);

}
