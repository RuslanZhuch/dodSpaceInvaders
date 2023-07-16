#include "CommonContext.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::Common
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("workspace/contexts/common.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 3) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };

        Engine::ContextUtils::loadVariable(this->width, loadingDataArray, 0);
        Engine::ContextUtils::loadVariable(this->height, loadingDataArray, 1);
        Engine::ContextUtils::loadVariable(this->title, loadingDataArray, 2);


        int32_t needBytes{};

        this->memory.allocate(needBytes);
        int32_t header{};


    }

    void Data::reset() noexcept
    {
    }

    void Data::merge(const Data& other) noexcept
    {
    }

}
