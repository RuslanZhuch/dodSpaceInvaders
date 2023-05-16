#include "PlayerGameplay.h"

#include "game/Inputs.h"
#include "game/CollisionCore.h"

#include <dod/BufferUtils.h>

#include <SFML/Window/Keyboard.hpp>

uint32_t gatherInputs()
{

    constexpr uint32_t moveLeftControlBit{ uint32_t(1) << 0 };
    constexpr uint32_t moveRightControlBit{ uint32_t(1) << 1 };
    constexpr uint32_t fireControlBit{ uint32_t(1) << 2 };

    uint32_t inputBits{};

    inputBits |= (moveLeftControlBit)*sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    inputBits |= (moveRightControlBit)*sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    inputBits |= (fireControlBit)*sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

    return inputBits;

}

void Game::Gameplay::Player::updateInputs(uint32_t& inputBits, uint32_t& prevInputBits) noexcept
{

    const auto newInputBits{ gatherInputs() };
    prevInputBits = inputBits;
    inputBits = newInputBits;

}

void Game::Gameplay::Player::updateMovement(
    float& moveComponent,
    float& playerPositionX,
    uint32_t inputBits,
    uint32_t prevInputBits,
    float dt
) noexcept
{

    const auto newMoveComponent{ Game::Inputs::computeMoveComponent(inputBits, prevInputBits, moveComponent) };

    moveComponent = newMoveComponent;

    playerPositionX += moveComponent * 200.f * dt;

}

int32_t Game::Gameplay::Player::updateFireComponent(
    int32_t playerLifes,
    uint32_t inputBits, 
    uint32_t prevInputBits
) noexcept
{
    const auto fireComponent{ Game::Inputs::computeFireComponent(inputBits, prevInputBits) };
    const auto bNeedCreateBullet{ (fireComponent > 0) && (playerLifes > 0) };

    return static_cast<int32_t>(bNeedCreateBullet);
}

void Game::Gameplay::Player::createBulletsSFx(
    Dod::DBBuffer<int32_t>& soundIds, 
    int32_t numOfBulletsCreated
) noexcept
{
    Dod::BufferUtils::populate(soundIds, 0, numOfBulletsCreated > 0);
}

void Game::Gameplay::Player::createBullets(
    int32_t numOfBulletsToCreate,
    Dod::DBBuffer<float>& bulletsXCoords,
    Dod::DBBuffer<float>& bulletsYCoords,
    float playerPositionX,
    float playerPositionY
) noexcept
{

    const auto bNeedCreateBullet{ numOfBulletsToCreate > 0 };
    Dod::BufferUtils::populate(bulletsXCoords, playerPositionX, bNeedCreateBullet);
    Dod::BufferUtils::populate(bulletsYCoords, playerPositionY, bNeedCreateBullet);

}

void Game::Gameplay::Player::testWithBullets(
    Dod::DBBuffer<int32_t>& bulletsToDelete,
    Dod::ImBuffer<float> bulletsXCorrds,
    Dod::ImBuffer<float> bulletsYCorrds,
    int32_t& playerLifes,
    float playerX,
    float playerY,
    float playerWidth,
    float playerHeight
) noexcept
{

    const auto prevNumBulletsToDelete{ Dod::BufferUtils::getNumFilledElements(bulletsToDelete) };

    Game::Core::Collision::pointsSquareIntersection(
        bulletsToDelete,
        bulletsXCorrds,
        bulletsYCorrds,
        playerX,
        playerY,
        playerWidth,
        playerHeight
    );

    const auto currNumBulletsToDelete{ Dod::BufferUtils::getNumFilledElements(bulletsToDelete) };

    playerLifes -= int32_t(1) * (currNumBulletsToDelete - prevNumBulletsToDelete > 0);

}
