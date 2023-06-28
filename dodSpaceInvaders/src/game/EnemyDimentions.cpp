#include "EnemyDimentions.h"

#include <engine/contextUtils.h>

#include <rapidjson/document.h>

#include <iostream>
#include <fstream>
#include <cassert>
#include <optional>
#include <functional>

void Game::Context::EnemyDimentions::Data::load() noexcept
{

    const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/enemyDimentions.json") };
    const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 2) };

    if (!inputDataOpt.has_value())
        return;

    const auto& loadingDataArray{ inputDataOpt.value() };

    Engine::ContextUtils::loadVariable(this->width, loadingDataArray, 0);
    Engine::ContextUtils::loadVariable(this->height, loadingDataArray, 1);

}

void Game::Context::EnemyDimentions::Data::reset() noexcept
{

}

void Game::Context::EnemyDimentions::Data::merge(const Data& other) noexcept
{

}
