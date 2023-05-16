#include "EnemiesGameplay.h"
#include "EnemiesCore.h"

auto Game::Gameplay::Enemies::enemiesBatchUpdate(
    float batchDirection, 
    float batchTargetX, 
    float batchTargetY, 
    bool strobe
)
{

    const auto bNeedChangeBatchDirection{ Game::Core::Enemies::computeNeedChangeDirection(
        batchDirection, batchTargetX, 75.f, 275.f) && strobe };

    const auto newBatchDirection{ Game::Core::Enemies::computeNewDirection(batchDirection, bNeedChangeBatchDirection) };
    const auto newBatchXCoord{ Game::Core::Enemies::computeNewXPosition(newBatchDirection, batchTargetX, 25.f, strobe) };

    const auto newBatchYCoord{ Game::Core::Enemies::computeNewYPosition(batchTargetY, 25.f, bNeedChangeBatchDirection) };

    struct Output
    {
        float batchXCoord{};
        float batchYCoord{};
        float batchDirection{};
    };
    return Output(newBatchXCoord, newBatchYCoord, newBatchDirection);

}

void Game::Gameplay::Enemies::enemiesLifetimeUpdate(
    Dod::DBBuffer<int32_t>& enemiesToRemove, 
    Dod::DBBuffer<float>& enemiesX,
    Dod::DBBuffer<float>& enemiesY
    )
{

    const auto enemiesToRemoveConst{ Dod::BufferUtils::createImFromBuffer(enemiesToRemove) };

    Dod::BufferUtils::remove(enemiesX, enemiesToRemoveConst);
    Dod::BufferUtils::remove(enemiesY, enemiesToRemoveConst);

    enemiesToRemove.numOfFilledEls = 0;

}

void Game::Gameplay::Enemies::enemiesUpdate(
    float dt, 
    float& batchMoveTimeleft,
    float& moveDirection,
    float& batchTargetX,
    float& batchCoordX,
    float& batchCoordY,
    Dod::DBBuffer<float>& enemiesXCoords,
    Dod::DBBuffer<float>& enemiesYCoords
)
{

    const auto newBatchMoveTimeLeft{ Game::Core::Enemies::updateStrobeCountdown(dt, batchMoveTimeleft, 1.f) };
    const auto bNeedMove{ Game::Core::Enemies::updateStrobe(batchMoveTimeleft, newBatchMoveTimeLeft) };
    batchMoveTimeleft = newBatchMoveTimeLeft;

    const auto [newBatchTargetX, newBatchCoordY, newCurrentDirection] {
        Game::Gameplay::Enemies::enemiesBatchUpdate(moveDirection, batchTargetX, batchCoordY, bNeedMove)};
    moveDirection = newCurrentDirection;
    batchTargetX = newBatchTargetX;

    const auto moveXDelata{ newBatchTargetX - batchCoordX };
    const auto moveYDelata{ newBatchCoordY - batchCoordY };

    const auto moveX{ moveXDelata * 10.f * dt };
    const auto newBatchCoordX{ batchCoordX + moveX };

    batchCoordX = newBatchCoordX;
    batchCoordY = newBatchCoordY;

    for (int32_t elId{ 0 }; elId < Dod::BufferUtils::getNumFilledElements(enemiesXCoords); ++elId)
        Dod::BufferUtils::get(enemiesXCoords, elId) += moveX;

    for (int32_t elId{ 0 }; elId < Dod::BufferUtils::getNumFilledElements(enemiesYCoords); ++elId)
        Dod::BufferUtils::get(enemiesYCoords, elId) += moveYDelata;

}

int32_t Game::Gameplay::Enemies::updateEnemyBulletsCreation(
    float dt, 
    float& enemyWeaponCooldownTimeLeft,
    int32_t numOfEnemiesAlive
) noexcept
{

    const auto newCooldownTimeLeft{ Game::Core::Enemies::updateStrobeCountdown(dt, enemyWeaponCooldownTimeLeft, 0.5f) };
    const auto bNeedCreateBullet{ Game::Core::Enemies::updateStrobe(enemyWeaponCooldownTimeLeft, newCooldownTimeLeft) };
    enemyWeaponCooldownTimeLeft = newCooldownTimeLeft;

    return static_cast<int32_t>(bNeedCreateBullet && numOfEnemiesAlive > 0);

}

void Game::Gameplay::Enemies::createBulletsSFx(
    Dod::DBBuffer<int32_t>& soundIds, 
    int32_t numOfBulletsToCreate
) noexcept
{

    while (numOfBulletsToCreate-- > 0) {
        Dod::BufferUtils::populate(soundIds, 1, true);
    }

}

void Game::Gameplay::Enemies::generateEnemyBullets(
    int32_t numOfBulletsToCreate,

    Dod::DBBuffer<float>& bulletsXCoords,
    Dod::DBBuffer<float>& bulletsYCoords,
    
    Game::Utils::RandomGenerator& rand,
    
    const Dod::ImBuffer<float> enemiesXCoords,
    const Dod::ImBuffer<float> enemiesYCoords
)
{

    Game::Core::Enemies::generateBullet(bulletsXCoords, enemiesXCoords, rand, numOfBulletsToCreate > 0);
    Game::Core::Enemies::generateBullet(bulletsYCoords, enemiesYCoords, rand, numOfBulletsToCreate > 0);

}

void Game::Gameplay::Enemies::generateEnemies(
    const int32_t numOfEnemiesPerRow,
    const int32_t numOfEnemiesCols,
    const float enemiesXStride,
    const float enemiesYStride,
    float& batchXCoord,
    float& batchYCoord,
    Dod::DBBuffer<float>& xCoords,
    Dod::DBBuffer<float>& yCoords
)
{
//    batchXCoord = 100.f;
//    batchYCoord = 200.f;

    Game::Core::Enemies::generateX(
        xCoords,
        numOfEnemiesPerRow, numOfEnemiesCols,
        batchXCoord, enemiesXStride
    );
    Game::Core::Enemies::generateY(
        yCoords,
        numOfEnemiesPerRow, numOfEnemiesCols,
        batchYCoord, enemiesYStride
    );
}
