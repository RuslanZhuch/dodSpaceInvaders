#include "PlayerBulletsContext.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::PlayerBullets
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("workspace/contexts/playerBullets.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 1) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };

        Engine::ContextUtils::loadVariable(this->velocity, loadingDataArray, 0);


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
