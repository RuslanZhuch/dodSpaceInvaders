#include "RenderInternal.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

void Game::Context::RenderInternal::Data::load() noexcept
{

    const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/renderInternal.json") };
    const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 1) };

    if (!inputDataOpt.has_value())
        return;

    const auto& loadingDataArray{ inputDataOpt.value() };

    const auto idsToRenderCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 0) };

    int32_t needBytes{};
    needBytes += idsToRenderCapacityBytes;

    this->memory.allocate(needBytes);
    int32_t header{};

    Engine::ContextUtils::loadBuffer(this->idsToRender, idsToRenderCapacityBytes, this->memory, header);

}

void Game::Context::RenderInternal::Data::reset() noexcept
{
    Dod::BufferUtils::flush(this->idsToRender);
}

void Game::Context::RenderInternal::Data::merge(const Data& other) noexcept
{
    Dod::BufferUtils::append(this->idsToRender, Dod::BufferUtils::createImFromBuffer(other.idsToRender));
}