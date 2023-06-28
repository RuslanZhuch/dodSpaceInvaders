#include "PlayerContext.h"

#include <engine/contextUtils.h>

#include <rapidjson/document.h>

#include <iostream>
#include <fstream>
#include <cassert>
#include <optional>
#include <functional>

void Game::Context::Player::Data::load() noexcept
{

    const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/playerContext.json") };
    const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 7) };

    if (!inputDataOpt.has_value())
        return;

    const auto& loadingDataArray{ inputDataOpt.value() };

    Engine::ContextUtils::loadVariable(this->xCoord, loadingDataArray, 0);
    Engine::ContextUtils::loadVariable(this->yCoord, loadingDataArray, 1);
    Engine::ContextUtils::loadVariable(this->lifes, loadingDataArray, 2);
    Engine::ContextUtils::loadVariable(this->inputs, loadingDataArray, 3);
    Engine::ContextUtils::loadVariable(this->prevInputs, loadingDataArray, 4);
    Engine::ContextUtils::loadVariable(this->move, loadingDataArray, 5);
    Engine::ContextUtils::loadVariable(this->fire, loadingDataArray, 6);

}

void Game::Context::Player::Data::reset() noexcept
{

}

void Game::Context::Player::Data::merge(const Data& other) noexcept
{

}
