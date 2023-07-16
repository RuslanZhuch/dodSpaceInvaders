#include "PlayerContext.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::Player
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("workspace/contexts/player.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 7) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };

        Engine::ContextUtils::loadVariable(this->positionX, loadingDataArray, 0);
        Engine::ContextUtils::loadVariable(this->positionY, loadingDataArray, 1);
        Engine::ContextUtils::loadVariable(this->lifes, loadingDataArray, 2);
        Engine::ContextUtils::loadVariable(this->inputs, loadingDataArray, 3);
        Engine::ContextUtils::loadVariable(this->prevInputs, loadingDataArray, 4);
        Engine::ContextUtils::loadVariable(this->move, loadingDataArray, 5);
        Engine::ContextUtils::loadVariable(this->fire, loadingDataArray, 6);


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
