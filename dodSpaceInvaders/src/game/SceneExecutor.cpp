#include "SceneExecutor.h"

#include <dod/BufferUtils.h>

#include <iostream>

void Game::ExecutionBlock::Scene::loadContext()
{
    this->renderContext.load();
}

void Game::ExecutionBlock::Scene::initiate()
{
    Dod::BufferUtils::constructBack(this->renderContext.modelsMeta, Context::RenderCommands::Data::ModelMeta(5, 1));

    Dod::BufferUtils::constructBack(this->renderContext.xCoords, 0.f);
    Dod::BufferUtils::constructBack(this->renderContext.yCoords, 0.f);
}

void Game::ExecutionBlock::Scene::update(float dt)
{

}
