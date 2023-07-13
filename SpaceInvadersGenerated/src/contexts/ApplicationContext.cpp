#include "ApplicationContext.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::Application
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("../workspace/contexts/application.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 1) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };


        const auto commandsCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 0) };

        int32_t needBytes{};
        needBytes += commandsCapacityBytes;

        this->memory.allocate(needBytes);
        int32_t header{};

        Engine::ContextUtils::loadBuffer(this->commands, commandsCapacityBytes, this->memory, header);

    }

    void Data::reset() noexcept
    {
        Dod::BufferUtils::flush(this->commands);
    }

    void Data::merge(const Data& other) noexcept
    {
        Dod::BufferUtils::append(this->commands, Dod::BufferUtils::createImFromBuffer(other.commands));
    }

}
