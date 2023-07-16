#include "SoundsContext.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::Sounds
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("workspace/contexts/sounds.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 2) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };

        Engine::ContextUtils::loadVariable(this->soundsCore, loadingDataArray, 0);

        const auto soundsCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<Game::Sound::Wav>(loadingDataArray, 1) };

        int32_t needBytes{};
        needBytes += soundsCapacityBytes;

        this->memory.allocate(needBytes);
        int32_t header{};

        Engine::ContextUtils::loadBuffer(this->sounds, soundsCapacityBytes, this->memory, header);

    }

    void Data::reset() noexcept
    {
        Dod::BufferUtils::flush(this->sounds);
    }

    void Data::merge(const Data& other) noexcept
    {
        Dod::BufferUtils::append(this->sounds, Dod::BufferUtils::createImFromBuffer(other.sounds));
    }

}
