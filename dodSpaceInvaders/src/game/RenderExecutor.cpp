#include "RenderExecutor.h"

#include <dod/Algorithms.h>

#include <rapidjson/document.h>
#include <rapidjson/reader.h>

#include <iostream>
#include <fstream>
#include <cassert>
#include <ranges>

void Game::ExecutionBlock::Render::loadContext()
{

    this->commonContext.load();
    this->applicationContext.load();
    this->renderBufferContext.load();

}

void Game::ExecutionBlock::Render::initiate()
{

    this->renderBufferContext.gameRenderer = std::make_unique<GameRenderer>(
        this->commonContext.width, 
        this->commonContext.height, 
        this->commonContext.title.data()
    );

}

void Game::ExecutionBlock::Render::update(float dt)
{

    sf::Event event;

    auto& window{ this->renderBufferContext.gameRenderer->getWindow() };
    while (window.pollEvent(event))
    {
        Dod::BufferUtils::populate(this->applicationContext.commands, 1, event.type == sf::Event::Closed);
    }

    window.clear();

    const auto xCoords{ Dod::SharedContext::get(this->sRenderContext).xCoords };
    const auto yCoords{ Dod::SharedContext::get(this->sRenderContext).yCoords };
    const auto modelsMeta{ Dod::SharedContext::get(this->sRenderContext).modelsMeta };

    const auto modelsData{ Dod::SharedContext::get(this->sModelsContext).loadedModels };
    const auto modelsDataIds{ Dod::SharedContext::get(this->sModelsContext).modelIds };

    int32_t modelElId{};
    for (int32_t metaElId{}; metaElId < Dod::BufferUtils::getNumFilledElements(modelsMeta); ++metaElId)
    {

        Dod::BufferUtils::flush(this->renderBufferContext.idsToRender);
        const auto modelIdToRender{ Dod::BufferUtils::get(modelsMeta, metaElId).modelId };
        for (int32_t existElId{}; existElId < Dod::BufferUtils::getNumFilledElements(modelsDataIds); ++existElId)
        {
            Dod::BufferUtils::populate(this->renderBufferContext.idsToRender, existElId,
                Dod::BufferUtils::get(modelsDataIds, existElId) == modelIdToRender);
        }

        const auto numOfInstances{ Dod::BufferUtils::get(modelsMeta, metaElId).numOfElements };
        for (int32_t instanceId{}; instanceId < numOfInstances; ++instanceId, ++modelElId)
        {
            const auto x{ Dod::BufferUtils::get(xCoords, modelElId) };
            const auto y{ Dod::BufferUtils::get(yCoords, modelElId) };
            sf::Transform transform;
            transform.translate({ x, y });
            for (int32_t renderElId{}; renderElId < Dod::BufferUtils::getNumFilledElements(this->renderBufferContext.idsToRender); ++renderElId)
            {
                window.draw(Dod::BufferUtils::get(modelsData, Dod::BufferUtils::get(this->renderBufferContext.idsToRender, renderElId)), transform);
            }
        }

    }

    window.display();

}

void Game::ExecutionBlock::Render::flushSharedLocalContexts()
{
    this->applicationContext.reset();
}