#include "Application.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

#include <rapidjson/document.h>

#include <iostream>
#include <fstream>
#include <cassert>

void Game::Context::Application::Data::load() noexcept
{

    const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/application.json") };
    const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 1) };

    if (!inputDataOpt.has_value())
        return;

    const auto& loadingDataArray{ inputDataOpt.value() };

    const auto commandsCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 0) };

    int32_t needBytes{};
    needBytes += commandsCapacityBytes;

    this->memory.allocate(needBytes);
    int32_t header{};

    Engine::ContextUtils::loadBuffer(this->commands, commandsCapacityBytes, this->memory, header);

}

void Game::Context::Application::Data::reset() noexcept
{
    Dod::BufferUtils::flush(this->commands);
}

void Game::Context::Application::Data::merge(const Data& other) noexcept
{
    Dod::BufferUtils::append(this->commands, Dod::BufferUtils::createImFromBuffer(other.commands));
}