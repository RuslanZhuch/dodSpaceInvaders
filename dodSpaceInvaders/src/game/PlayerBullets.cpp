#include "PlayerBullets.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::PlayerBullets
{

    void Data::load() noexcept
    {
        const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/playerBulletsContext.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 1) };

        if (!inputDataOpt.has_value())
            return;

        const auto& loadingDataArray{ inputDataOpt.value() };

        Engine::ContextUtils::loadVariable(this->velocity, loadingDataArray, 0);
    }

    void Data::reset() noexcept
    {

    }

    void Data::merge(const Data& other) noexcept
    {

    }

}
