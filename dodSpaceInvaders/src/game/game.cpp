
#include "game.h"
#include "../renderer/instantRenderer.h"
#include <SFML/Graphics.hpp>
#include <span>
#include <array>
#include <cassert>
#include <chrono>
#include <range/v3/view.hpp>
#include <range/v3/span.hpp>
#include <range/v3/view/zip.hpp>

constexpr auto numOfCells{ 20 };
constexpr auto filedSizeCoeff{ 0.95f };

const auto getFieldSize(sf::RenderWindow& window)
{

    const auto windowSize{ window.getSize() };
    return sf::Vector2f(windowSize.x * filedSizeCoeff, windowSize.y * filedSizeCoeff);

}
const auto getCornerPoint(sf::RenderWindow& window)
{

    const auto windowSize{ window.getSize() };
    return sf::Vector2f(windowSize.x, windowSize.x) * (1.f - filedSizeCoeff) * 0.5f;

}

void drawField(sf::RenderWindow& window)
{

    const auto windowSize{ window.getSize() };
    const auto centerPoint{ sf::Vector2f(windowSize) * 0.5f };
    const auto fieldSize{ sf::Vector2f(windowSize) * filedSizeCoeff };

    const auto color{ sf::Color(200, 100, 50, 200) };
    Renderer::Instant::drawRectangle(window, centerPoint, fieldSize, color, 2.f);

}

void drawPlayer(sf::RenderWindow& window, const sf::Vector2f& position)
{

    constexpr auto radius{ 25.f };
    const auto leftPoint{ position - sf::Vector2f(-radius, 0.f) };
    const auto rightPoint{ position - sf::Vector2f(radius, 0.f) };
    const auto topPoint{ position - sf::Vector2f(0.f, radius) };

    const auto playerColor{ sf::Color(150.f, 200.f, 90.f) };
    Renderer::Instant::drawLine(window, leftPoint, rightPoint, playerColor);
    Renderer::Instant::drawLine(window, leftPoint, topPoint, playerColor);
    Renderer::Instant::drawLine(window, rightPoint, topPoint, playerColor);

}

void drawBullet(sf::RenderWindow& window, const sf::Vector2f& position)
{
    constexpr auto length{ 15.f };
    Renderer::Instant::drawRectangle(window, position - sf::Vector2f(0.f, length), sf::Vector2f(6.f, length), sf::Color::Yellow, 1.f, sf::Color::Green);
}

void drawBullets(sf::RenderWindow& window, const std::span<const float> xPositions, const std::span<const float> yPositions)
{
    for (const auto [x, y] : ranges::views::zip(xPositions.subspan(1), yPositions.subspan(1)))
    {
        drawBullet(window, { x, y });
    }
}

struct ControlState
{
    float movementComponent{ 0.f };
    int32_t fireComponent{ 0 };
};

constexpr int32_t moveLeftControlBit{ int32_t(1) << 0 };
constexpr int32_t moveRightControlBit{ int32_t(1) << 1 };
constexpr int32_t fireControlBit{ int32_t(1) << 2 };

uint32_t gatherInputs()
{

    uint32_t inputBits{};

    inputBits |= (moveLeftControlBit)*sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    inputBits |= (moveRightControlBit)*sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    inputBits |= (fireControlBit)*sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

    return inputBits;

}

static ControlState gCurrentControlState;


ControlState inputsUpdate(ControlState currentControlState, const uint32_t newInputBits, const uint32_t prevInputBits)
{

    struct PlayerControlMoveRule
    {
        struct Conditions
        {
            uint32_t inputKey{};
            uint32_t prevInputKey{};
            uint32_t mask{};
        };
        std::array<Conditions, 4> conditions{ {
            Conditions(0, moveLeftControlBit, ~(moveLeftControlBit | moveRightControlBit)),
            Conditions(0, moveRightControlBit, ~(moveLeftControlBit | moveRightControlBit)),
            Conditions(moveLeftControlBit, ~moveLeftControlBit, ~moveLeftControlBit),
            Conditions(moveRightControlBit, ~moveRightControlBit, ~moveRightControlBit),
        } };
        std::array<float, 4> movement{ {
            0.f,
            0.f,
            -1.f,
            1.f,
        } };
    };
    struct PlayerControlFireRule
    {
        struct Conditions
        {
            uint32_t inputKey{};
            uint32_t prevInputKey{};
            uint32_t mask{};
        };
        std::array<Conditions, 3> conditions{ {
            Conditions(0, fireControlBit, ~fireControlBit),
            Conditions(fireControlBit, fireControlBit, ~fireControlBit),
            Conditions(fireControlBit, ~fireControlBit, ~fireControlBit),
            } };
        std::array<int32_t, 3> fire{ {0, 0, 1} };
    };

    ControlState newState{ gCurrentControlState };

    {
        PlayerControlMoveRule controlRule;

        for (size_t idx{ 0 }; idx < controlRule.conditions.size(); ++idx)
        {
            const auto cr1{ !((controlRule.conditions[idx].inputKey ^ newInputBits) & ~controlRule.conditions[idx].mask)};
            const auto cr2{ !((controlRule.conditions[idx].prevInputKey ^ prevInputBits) & ~controlRule.conditions[idx].mask) };
            const auto satisfy{ cr1 && cr2 };

            if (satisfy)
                newState.movementComponent = controlRule.movement[idx] * satisfy + gCurrentControlState.movementComponent * !satisfy;
            
        }
    }

    {
        PlayerControlFireRule controlRule;

        for (size_t idx{ 0 }; idx < controlRule.conditions.size(); ++idx)
        {
            const auto satisfy{
                !((controlRule.conditions[idx].inputKey ^ newInputBits) & ~controlRule.conditions[idx].mask) &&
                !((controlRule.conditions[idx].prevInputKey ^ prevInputBits) & ~controlRule.conditions[idx].mask)
            };
            if (satisfy)
                newState.fireComponent = controlRule.fire[idx] * satisfy;
        }
    }

    return newState;

}

size_t gNumOfPlayerBullets{ 0 };

std::vector<float> gPlayerBulletXCoords;
std::vector<float> gPlayerBulletYCoords;
std::vector<float> gPlayerBulletTimeLeft;

size_t gNumOfRemovedBullets{ 0 };
std::vector<size_t> gRemovedBulletIds;

void bulletsUpdate(float dt)
{

    constexpr auto bulletSpeed{ 220.f };
    for (size_t idx{ 1 }; idx < gPlayerBulletYCoords.size(); ++idx) {
        gPlayerBulletYCoords[idx] -= bulletSpeed * dt;
    }

    size_t bulletToRemoveId{ 0 };
    for (size_t idx{ 1 }; idx < gPlayerBulletTimeLeft.size(); ++idx) {
        gPlayerBulletTimeLeft[idx] -= dt;
        bulletToRemoveId += idx * (gPlayerBulletTimeLeft[idx] <= 0.f);
    }

    const auto swapLeftId{ bulletToRemoveId };
    const auto swapRightId{ gNumOfPlayerBullets * (bulletToRemoveId > 0) };

    std::swap(gPlayerBulletTimeLeft[swapLeftId], gPlayerBulletTimeLeft[swapRightId]);
    std::swap(gPlayerBulletXCoords[swapLeftId], gPlayerBulletXCoords[swapRightId]);
    std::swap(gPlayerBulletYCoords[swapLeftId], gPlayerBulletYCoords[swapRightId]);

     gNumOfPlayerBullets -= 1 * (bulletToRemoveId > 0);

}

static sf::Vector2f gPlayerPosition;
static int32_t prevInputBits;

void playerUpdate(float dt)
{

    const auto inputBits{ gatherInputs() };
//    const auto fakeInputBits{ prevInputBits | inputBits };
    
    const auto controlsState{ inputsUpdate(gCurrentControlState, inputBits, prevInputBits) };
    prevInputBits = inputBits;

    gCurrentControlState = controlsState;

    gPlayerPosition.x += controlsState.movementComponent * 200.f * dt;

    const auto bNeedCreateBullet{ controlsState.fireComponent > 0 };

    const auto newBulletIdx{ bNeedCreateBullet * (gNumOfPlayerBullets + 1) };

    gPlayerBulletXCoords.resize(1 + gNumOfPlayerBullets + 1 * bNeedCreateBullet);
    gPlayerBulletYCoords.resize(1 + gNumOfPlayerBullets + 1 * bNeedCreateBullet);
    gPlayerBulletTimeLeft.resize(1 + gNumOfPlayerBullets + 1 * bNeedCreateBullet);

    gPlayerBulletXCoords[newBulletIdx] = gPlayerPosition.x;
    gPlayerBulletYCoords[newBulletIdx] = gPlayerPosition.y;
    gPlayerBulletTimeLeft[newBulletIdx] = 3.f;
    gNumOfPlayerBullets += size_t(1) * bNeedCreateBullet;

}

void msgLoop(sf::RenderWindow& window, float dt)
{

    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }

    window.clear();

    playerUpdate(dt);
    bulletsUpdate(dt);

    drawField(window);
    drawPlayer(window, gPlayerPosition);

    drawBullets(window, gPlayerBulletXCoords, gPlayerBulletYCoords);

//    drawFood(window, food);
//    drawSnake(window, snake);

    window.display();

}

void Game::run()
{

    sf::RenderWindow window(sf::VideoMode(800, 900), "Starhammer");
    gPlayerPosition = sf::Vector2f(400.f, 850.f);

    gPlayerBulletXCoords.emplace_back();
    gPlayerBulletYCoords.emplace_back();

    float deltaTime{ 0.f };

    while (window.isOpen())
    {
        const auto start{ std::chrono::high_resolution_clock::now() };
        msgLoop(window, deltaTime);
        const auto end{ std::chrono::high_resolution_clock::now() };
        deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1'000'000'000.f;
    }

}
