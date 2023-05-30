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
    int32_t header{ 0 };

    {
        std::ifstream contextFile("resources/contexts/commonContext.json");
        assert(contextFile.is_open());
        const std::string fileRawData((std::istreambuf_iterator<char>(contextFile)), std::istreambuf_iterator<char>());

        rapidjson::Document json;
        json.Parse(fileRawData.c_str());
        const auto root{ json.GetObject() };

        for (const auto& element : root)
        {
            if (element.name == "contextName")
            {
                const auto contextName{ element.value.GetString() };
                std::cout << "Loading context: " << contextName << "\n";
            }
            else if (element.name == "data")
            {
                for (const auto& dataElement : element.value.GetObject())
                {
                    if (dataElement.name == "applicationContext" && dataElement.value.IsObject())
                    {
                        const auto applicationContextObj{ dataElement.value.GetObject() };
                        const auto totalMemoryBytes{ applicationContextObj["totalMemoryBytes"].GetInt() };
                        this->memory.allocate(totalMemoryBytes);
                        const auto windowWidth{ applicationContextObj["windowWidth"].GetInt() };
                        this->commonContext.width = windowWidth;
                        const auto windowHeight{ applicationContextObj["windowHeight"].GetInt() };
                        this->commonContext.height = windowHeight;
                        const auto title{ applicationContextObj["title"].GetString() };
                        const auto titleSize{ applicationContextObj["title"].GetStringLength() };
                        std::memcpy(this->commonContext.title.data(), title, std::min(this->commonContext.title.size(), static_cast<size_t>(titleSize)));
                    }
                }
            }
        }
    }


    {
        std::ifstream contextFile("resources/contexts/rendererContext.json");
        assert(contextFile.is_open());
        const std::string fileRawData((std::istreambuf_iterator<char>(contextFile)), std::istreambuf_iterator<char>());

        rapidjson::Document json;
        json.Parse(fileRawData.c_str());
        const auto root{ json.GetObject() };

        for (const auto& element : root)
        {
            if (element.name == "contextName")
            {
                const auto contextName{ element.value.GetString() };
                std::cout << "Loading context: " << contextName << "\n";
            }
            else if (element.name == "data")
            {
                for (const auto& dataElement : element.value.GetObject())
                {
                    if (dataElement.name == "renderBuffer" && dataElement.value.IsObject())
                    {
                        const auto renderBuffertObj{ dataElement.value.GetObject() };

                        const auto idsToRender{ renderBuffertObj["idsToRender"].GetObject() };
                        const auto idsToRenderType{ idsToRender["type"].GetString() };
                        const auto idsToRenderDataType{ idsToRender["dataType"].GetString() };
                        const auto idsToRenderCapacity{ idsToRender["capacity"].GetInt() };
                        const auto idsToRenderCapacityBytes{ idsToRenderCapacity * sizeof(int32_t) };
                        Dod::BufferUtils::initFromMemory(this->renderBufferContext.idsToRender, Dod::MemUtils::stackAquire(this->memory, idsToRenderCapacityBytes, header));
                    }
                }
            }
        }
    }

}

void Game::ExecutionBlock::Render::initiate()
{

	this->gameRenderer = std::make_unique<GameRenderer>(
        this->commonContext.width, 
        this->commonContext.height, 
        this->commonContext.title.data()
    );

}

bool Game::ExecutionBlock::Render::update(float dt)
{

    sf::Event event;

    auto& window{ this->gameRenderer->getWindow() };
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
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

    return window.isOpen();

}
