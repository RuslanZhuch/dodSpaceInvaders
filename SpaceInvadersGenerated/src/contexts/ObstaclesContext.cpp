#include "ObstaclesContext.h"

#include <dod/BufferUtils.h>
#include <engine/contextUtils.h>

namespace Game::Context::Obstacles
{

    void Data::load() noexcept
    {

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("workspace/contexts/obstacles.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 10) };

        if (!inputDataOpt.has_value())
        {
            return;
        }

        const auto& loadingDataArray{ inputDataOpt.value() };

        Engine::ContextUtils::loadVariable(this->stride, loadingDataArray, 0);
        Engine::ContextUtils::loadVariable(this->perRow, loadingDataArray, 1);
        Engine::ContextUtils::loadVariable(this->perCol, loadingDataArray, 2);
        Engine::ContextUtils::loadVariable(this->numOfClusters, loadingDataArray, 3);
        Engine::ContextUtils::loadVariable(this->clustersTotalAreaWidth, loadingDataArray, 4);
        Engine::ContextUtils::loadVariable(this->clusterY, loadingDataArray, 5);
        Engine::ContextUtils::loadVariable(this->initialLifes, loadingDataArray, 6);
        Engine::ContextUtils::loadVariable(this->width, loadingDataArray, 7);

        const auto lifesCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 8) };
        const auto toRemoveCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 9) };

        int32_t needBytes{};
        needBytes += lifesCapacityBytes;
        needBytes += toRemoveCapacityBytes;

        this->memory.allocate(needBytes);
        int32_t header{};

        Engine::ContextUtils::loadBuffer(this->lifes, lifesCapacityBytes, this->memory, header);
        Engine::ContextUtils::loadBuffer(this->toRemove, toRemoveCapacityBytes, this->memory, header);

    }

    void Data::reset() noexcept
    {
        Dod::BufferUtils::flush(this->lifes);
        Dod::BufferUtils::flush(this->toRemove);
    }

    void Data::merge(const Data& other) noexcept
    {
        Dod::BufferUtils::append(this->lifes, Dod::BufferUtils::createImFromBuffer(other.lifes));
        Dod::BufferUtils::append(this->toRemove, Dod::BufferUtils::createImFromBuffer(other.toRemove));
    }

}
