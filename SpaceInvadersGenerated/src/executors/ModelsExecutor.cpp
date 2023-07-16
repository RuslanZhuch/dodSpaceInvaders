#include "ModelsExecutor.h"

namespace Game::ExecutionBlock
{

    void Models::loadContext() noexcept
    {
        this->playerParamsContext.load();
        this->sceneParamsContext.load();
        this->windowParamsContext.load();

        this->modelsContext.load();
    }

    void Models::initiate() noexcept
    {
        this->initImpl();
    }

    void Models::update(float dt) noexcept
    {
        this->updateImpl(dt);
    }

    void Models::flushSharedLocalContexts() noexcept
    {
        this->modelsContext.reset();
    }

}
