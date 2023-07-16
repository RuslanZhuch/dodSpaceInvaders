#include "ModelsToRenderContext.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::ModelsToRender
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("workspace/contexts/modelsToRender.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 2) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };


        const auto loadedModelsCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<sf::VertexArray>(loadingDataArray, 0) };
        const auto modelIdsCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 1) };

        int32_t needBytes{};
        needBytes += loadedModelsCapacityBytes;
        needBytes += modelIdsCapacityBytes;

        this->memory.allocate(needBytes);
        int32_t header{};

        Engine::ContextUtils::loadBuffer(this->loadedModels, loadedModelsCapacityBytes, this->memory, header);
        Engine::ContextUtils::loadBuffer(this->modelIds, modelIdsCapacityBytes, this->memory, header);

    }

    void Data::reset() noexcept
    {
        Dod::BufferUtils::flush(this->loadedModels);
        Dod::BufferUtils::flush(this->modelIds);
    }

    void Data::merge(const Data& other) noexcept
    {
        Dod::BufferUtils::append(this->loadedModels, Dod::BufferUtils::createImFromBuffer(other.loadedModels));
        Dod::BufferUtils::append(this->modelIds, Dod::BufferUtils::createImFromBuffer(other.modelIds));
    }

}
