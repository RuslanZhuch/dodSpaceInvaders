#include "ObstacleParameter.h"

#include <engine/contextUtils.h>

namespace Game::Context::ObstacleParameters
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/obstacleParameters.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 2) };

        if (!inputDataOpt.has_value())
            return;

        const auto& loadingDataArray{ inputDataOpt.value() };

        Engine::ContextUtils::loadVariable(this->width, loadingDataArray, 0);
        Engine::ContextUtils::loadVariable(this->height, loadingDataArray, 1);

    }

    void Data::reset() noexcept
    {

    }

    void Data::merge(const Data& other) noexcept
    {

    }

}
