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
    const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 9) };

    if (!inputDataOpt.has_value())
        return;

    const auto& loadingDataArray{ inputDataOpt.value() };

    Engine::ContextUtils::loadVariable(this->width, loadingDataArray, 0);
    Engine::ContextUtils::loadVariable(this->height, loadingDataArray, 1);
    Engine::ContextUtils::loadVariable(this->xCoord, loadingDataArray, 2);
    Engine::ContextUtils::loadVariable(this->yCoord, loadingDataArray, 3);
    Engine::ContextUtils::loadVariable(this->lifes, loadingDataArray, 4);
    Engine::ContextUtils::loadVariable(this->inputs, loadingDataArray, 5);
    Engine::ContextUtils::loadVariable(this->prevInputs, loadingDataArray, 6);
    Engine::ContextUtils::loadVariable(this->move, loadingDataArray, 7);
    Engine::ContextUtils::loadVariable(this->fire, loadingDataArray, 8);

}

void Game::Context::Player::Data::reset() noexcept
{

}

void Game::Context::Player::Data::merge(const Data& other) noexcept
{

}
