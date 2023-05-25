#include "ModelsExecutor.h"

template <>
const Game::Context::Models::Shared& Game::ExecutionBlock::Models::getSharedLocalContext<Game::Context::Models::Shared>()
{
    return this->modelsContext;
}

void Game::ExecutionBlock::Models::loadContext()
{
}

void Game::ExecutionBlock::Models::initiate()
{
}

bool Game::ExecutionBlock::Models::update(float dt)
{
    return true;
}

void Game::ExecutionBlock::Models::flushSharedLocalContexts()
{
    this->modelsContext.reset();
}
