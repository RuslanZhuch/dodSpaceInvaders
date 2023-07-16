#include "SceneExecutor.h"

#include <dod/BufferUtils.h>

namespace Game::ExecutionBlock
{

    void Scene::initImpl() noexcept
    {
        Dod::BufferUtils::constructBack(this->renderCmdsContext.modelsMeta, Models::ModelMeta(5, 1));

        Dod::BufferUtils::constructBack(this->renderCmdsContext.xCoords, 0.f);
        Dod::BufferUtils::constructBack(this->renderCmdsContext.yCoords, 0.f);
    }

    void Scene::updateImpl(float dt) noexcept
    {

    }

}
