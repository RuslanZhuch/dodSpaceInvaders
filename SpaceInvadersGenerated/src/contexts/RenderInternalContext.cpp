#include "RenderInternalContext.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::RenderInternal
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("workspace/contexts/renderInternal.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 2) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };

        Engine::ContextUtils::loadVariable(this->renderer, loadingDataArray, 0);

        const auto idsToRenderCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 1) };

        int32_t needBytes{};
        needBytes += idsToRenderCapacityBytes;

        this->memory.allocate(needBytes);
        int32_t header{};

        Engine::ContextUtils::loadBuffer(this->idsToRender, idsToRenderCapacityBytes, this->memory, header);

    }

    void Data::reset() noexcept
    {
        Dod::BufferUtils::flush(this->idsToRender);
    }

    void Data::merge(const Data& other) noexcept
    {
        Dod::BufferUtils::append(this->idsToRender, Dod::BufferUtils::createImFromBuffer(other.idsToRender));
    }

}
