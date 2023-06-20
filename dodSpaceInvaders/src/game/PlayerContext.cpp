#include "PlayerContext.h"

#include <engine/contextUtils.h>

#include <rapidjson/document.h>

#include <iostream>
#include <fstream>
#include <cassert>
#include <optional>
#include <functional>

Game::Context::Player::Data Game::Context::Player::Data::load(Dod::MemPool& pool, int32_t& header) noexcept
{

    Data data;

    const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/playerContext.json") };
    const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 9) };

    if (!inputDataOpt.has_value())
        return data;

    const auto& loadingDataArray{ inputDataOpt.value() };

    Engine::ContextUtils::loadVariable(data.width, loadingDataArray, 0);
    Engine::ContextUtils::loadVariable(data.height, loadingDataArray, 1);
    Engine::ContextUtils::loadVariable(data.xCoord, loadingDataArray, 2);
    Engine::ContextUtils::loadVariable(data.yCoord, loadingDataArray, 3);
    Engine::ContextUtils::loadVariable(data.lifes, loadingDataArray, 4);
    Engine::ContextUtils::loadVariable(data.inputs, loadingDataArray, 5);
    Engine::ContextUtils::loadVariable(data.prevInputs, loadingDataArray, 6);
    Engine::ContextUtils::loadVariable(data.move, loadingDataArray, 7);
    Engine::ContextUtils::loadVariable(data.fire, loadingDataArray, 8);

    return data;

}
