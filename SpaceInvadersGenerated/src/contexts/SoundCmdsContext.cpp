#include "SoundCmdsContext.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::SoundCmds
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("workspace/contexts/soundCmds.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 1) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };


        const auto soundIdsToPlayCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 0) };

        int32_t needBytes{};
        needBytes += soundIdsToPlayCapacityBytes;

        this->memory.allocate(needBytes);
        int32_t header{};

        Engine::ContextUtils::loadBuffer(this->soundIdsToPlay, soundIdsToPlayCapacityBytes, this->memory, header);

    }

    void Data::reset() noexcept
    {
        Dod::BufferUtils::flush(this->soundIdsToPlay);
    }

    void Data::merge(const Data& other) noexcept
    {
        Dod::BufferUtils::append(this->soundIdsToPlay, Dod::BufferUtils::createImFromBuffer(other.soundIdsToPlay));
    }

}
