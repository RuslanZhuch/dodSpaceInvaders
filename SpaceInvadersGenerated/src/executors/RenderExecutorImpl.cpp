#include "RenderExecutor.h"
#include <dod/BufferUtils.h>

#include <memory>

namespace Game::ExecutionBlock
{

    void Render::initImpl() noexcept
    {
        this->rendererContext.renderer = std::make_unique<Game::GameRenderer>(
            static_cast<int32_t>(this->windowSpecsContext.width),
            static_cast<int32_t>(this->windowSpecsContext.height),
            this->windowSpecsContext.title.data.data()
        );
    }

    void Render::updateImpl(float dt) noexcept
    {
        sf::Event event;

        auto& window{ this->rendererContext.renderer->getWindow() };
        while (window.pollEvent(event))
        {
            Dod::BufferUtils::populate(this->applicationContext.commands, 1, event.type == sf::Event::Closed);
        }

        window.clear();

        const auto xCoords{ Dod::SharedContext::get(this->renderCmdsContext).xCoords };
        const auto yCoords{ Dod::SharedContext::get(this->renderCmdsContext).yCoords };
        const auto modelsMeta{ Dod::SharedContext::get(this->renderCmdsContext).modelsMeta };

        const auto modelsData{ Dod::SharedContext::get(this->modelsIdsContext).loadedModels };
        const auto modelsDataIds{ Dod::SharedContext::get(this->modelsIdsContext).modelIds };

        int32_t modelElId{};
        for (int32_t metaElId{}; metaElId < Dod::BufferUtils::getNumFilledElements(modelsMeta); ++metaElId)
        {

            Dod::BufferUtils::flush(this->internalContext.idsToRender);
            const auto modelIdToRender{ Dod::BufferUtils::get(modelsMeta, metaElId).modelId };
            for (int32_t existElId{}; existElId < Dod::BufferUtils::getNumFilledElements(modelsDataIds); ++existElId)
            {
                Dod::BufferUtils::populate(this->internalContext.idsToRender, existElId,
                    Dod::BufferUtils::get(modelsDataIds, existElId) == modelIdToRender);
            }

            const auto numOfInstances{ Dod::BufferUtils::get(modelsMeta, metaElId).numOfElements };
            for (int32_t instanceId{}; instanceId < numOfInstances; ++instanceId, ++modelElId)
            {
                const auto x{ Dod::BufferUtils::get(xCoords, modelElId) };
                const auto y{ Dod::BufferUtils::get(yCoords, modelElId) };
                sf::Transform transform;
                transform.translate({ x, y });
                for (int32_t renderElId{}; renderElId < Dod::BufferUtils::getNumFilledElements(this->internalContext.idsToRender); ++renderElId)
                {
                    window.draw(Dod::BufferUtils::get(modelsData, Dod::BufferUtils::get(this->internalContext.idsToRender, renderElId)), transform);
                }
            }

        }

        window.display();
    }

}
