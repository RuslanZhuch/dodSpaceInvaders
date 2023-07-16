#include "SceneExecutor.h"

namespace Game::ExecutionBlock
{

    void Scene::loadContext() noexcept
    {

        this->renderCmdsContext.load();
    }

    void Scene::initiate() noexcept
    {
        this->initImpl();
    }

    void Scene::update(float dt) noexcept
    {
        this->updateImpl(dt);
    }

    void Scene::flushSharedLocalContexts() noexcept
    {
    }

}
