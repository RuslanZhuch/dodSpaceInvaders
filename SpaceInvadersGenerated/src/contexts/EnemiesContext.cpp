#include "EnemiesContext.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::Enemies
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("workspace/contexts/enemies.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 13) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };

        Engine::ContextUtils::loadVariable(this->numOfEnemiesPerRow, loadingDataArray, 0);
        Engine::ContextUtils::loadVariable(this->numOfEnemiesPerCol, loadingDataArray, 1);
        Engine::ContextUtils::loadVariable(this->enemiesXStride, loadingDataArray, 2);
        Engine::ContextUtils::loadVariable(this->enemiesYStride, loadingDataArray, 3);
        Engine::ContextUtils::loadVariable(this->weaponCooldownTime, loadingDataArray, 4);
        Engine::ContextUtils::loadVariable(this->weaponCooldownTimeLeft, loadingDataArray, 5);
        Engine::ContextUtils::loadVariable(this->startCoordX, loadingDataArray, 6);
        Engine::ContextUtils::loadVariable(this->startCoordY, loadingDataArray, 7);
        Engine::ContextUtils::loadVariable(this->direction, loadingDataArray, 8);
        Engine::ContextUtils::loadVariable(this->cooldownTimeLeft, loadingDataArray, 9);
        Engine::ContextUtils::loadVariable(this->cooldownTime, loadingDataArray, 10);
        Engine::ContextUtils::loadVariable(this->targetX, loadingDataArray, 11);
        Engine::ContextUtils::loadVariable(this->rand, loadingDataArray, 12);


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
