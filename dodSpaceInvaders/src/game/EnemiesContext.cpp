#include "EnemiesContext.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

#include <rapidjson/document.h>

#include <iostream>
#include <fstream>
#include <cassert>

void Game::Context::Enemy::Data::load() noexcept
{

    const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/enemiesContext.json") };
    const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 11) };

    if (!inputDataOpt.has_value())
        return;

    const auto& loadingDataArray{ inputDataOpt.value() };

    Engine::ContextUtils::loadVariable(this->numOfEnemiesPerRow, loadingDataArray, 0);
    Engine::ContextUtils::loadVariable(this->numOfEnemiesCols, loadingDataArray, 1);
    Engine::ContextUtils::loadVariable(this->enemiesXStride, loadingDataArray, 2);
    Engine::ContextUtils::loadVariable(this->enemiesYStride, loadingDataArray, 3);
    Engine::ContextUtils::loadVariable(this->weaponCooldownTime, loadingDataArray, 4);
    Engine::ContextUtils::loadVariable(this->batchCoordX, loadingDataArray, 5);
    Engine::ContextUtils::loadVariable(this->batchCoordY, loadingDataArray, 6);
    Engine::ContextUtils::loadVariable(this->direction, loadingDataArray, 7);
    Engine::ContextUtils::loadVariable(this->enemyWeaponCooldownTimeLeft, loadingDataArray, 8);
    Engine::ContextUtils::loadVariable(this->weaponCooldownTime, loadingDataArray, 9);

    const auto toRemoveCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 10) };

    int32_t needBytes{};
    needBytes += toRemoveCapacityBytes;

    this->memory.allocate(needBytes);
    int32_t header{};

    Engine::ContextUtils::loadBuffer(this->toRemove, toRemoveCapacityBytes, this->memory, header);

    return;

}

void Game::Context::Enemy::Data::reset() noexcept
{
    Dod::BufferUtils::flush(this->toRemove);
}

void Game::Context::Enemy::Data::merge(const Data& other) noexcept
{
    Dod::BufferUtils::append(this->toRemove, Dod::BufferUtils::createImFromBuffer(other.toRemove));
}
