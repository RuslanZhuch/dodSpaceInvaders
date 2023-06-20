#include "EnemiesContext.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

#include <rapidjson/document.h>

#include <iostream>
#include <fstream>
#include <cassert>

Game::Context::Enemy::Data Game::Context::Enemy::Data::load(Dod::MemPool& pool, int32_t& header) noexcept
{

    Data data;

    const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/enemiesContext.json") };
    const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 13) };

    if (!inputDataOpt.has_value())
        return data;

    const auto& loadingDataArray{ inputDataOpt.value() };

    Engine::ContextUtils::loadVariable(data.numOfEnemiesPerRow, loadingDataArray, 0);
    Engine::ContextUtils::loadVariable(data.numOfEnemiesCols, loadingDataArray, 1);
    Engine::ContextUtils::loadVariable(data.enemiesXStride, loadingDataArray, 2);
    Engine::ContextUtils::loadVariable(data.enemiesYStride, loadingDataArray, 3);
    Engine::ContextUtils::loadVariable(data.width, loadingDataArray, 4);
    Engine::ContextUtils::loadVariable(data.height, loadingDataArray, 5);
    Engine::ContextUtils::loadVariable(data.weaponCooldownTime, loadingDataArray, 6);
    Engine::ContextUtils::loadVariable(data.batchCoordX, loadingDataArray, 7);
    Engine::ContextUtils::loadVariable(data.batchCoordY, loadingDataArray, 8);
    Engine::ContextUtils::loadVariable(data.direction, loadingDataArray, 9);
    Engine::ContextUtils::loadVariable(data.enemyWeaponCooldownTimeLeft, loadingDataArray, 10);

    Engine::ContextUtils::loadBuffer(data.toRemove, loadingDataArray, 12, pool, header);

    return data;

}