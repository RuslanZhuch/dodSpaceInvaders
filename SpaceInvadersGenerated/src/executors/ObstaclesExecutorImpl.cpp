#include "ObstaclesExecutor.h"

#include <internal/gameplay/ObstaclesGameplay.h>
#include <internal/gameplay/ObstaclesCore.h>

#include <dod/BufferUtils.h>

namespace Game::ExecutionBlock
{

    void Obstacles::initImpl() noexcept
    {
        Game::Gameplay::Obstacles::create(
            this->obstaclesCoordsContext.xCoords,
            this->obstaclesCoordsContext.yCoords,
            this->parametersContext.lifes,
            this->parametersContext.perRow,
            this->parametersContext.perCol,
            this->parametersContext.stride,
            this->parametersContext.numOfClusters,
            this->parametersContext.clustersTotalAreaWidth,
            this->parametersContext.clusterY,
            this->parametersContext.initialLifes,
            this->parametersContext.width
        );
    }

    void Obstacles::updateImpl(float dt) noexcept
    {

        const auto obstaclesToHit{ Dod::SharedContext::get(this->toHitContext).ids };

        Game::Core::Obstacles::updateLifetime(
            this->toRemoveContext.ids,
            this->parametersContext.lifes,
            Dod::BufferUtils::createImFromBuffer(obstaclesToHit)
        );

        Game::Core::Obstacles::remove(
            this->toRemoveContext.ids,
            this->parametersContext.lifes,
            this->obstaclesCoordsContext.xCoords,
            this->obstaclesCoordsContext.yCoords
        );

        Dod::BufferUtils::constructBack(this->renderCmdsContext.modelsMeta);
        Dod::BufferUtils::get(this->renderCmdsContext.modelsMeta, 0).modelId = 3;
        Dod::BufferUtils::get(this->renderCmdsContext.modelsMeta, 0).numOfElements =
            Dod::BufferUtils::getNumFilledElements(this->obstaclesCoordsContext.xCoords);

        Dod::BufferUtils::append(this->renderCmdsContext.xCoords, Dod::BufferUtils::createImFromBuffer(this->obstaclesCoordsContext.xCoords));
        Dod::BufferUtils::append(this->renderCmdsContext.yCoords, Dod::BufferUtils::createImFromBuffer(this->obstaclesCoordsContext.yCoords));

    }

}
