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

    constexpr auto sizeX{ 25.f };
    constexpr auto sizeY{ 30.f };
    constexpr auto sizeXHalf{ sizeX * 0.5f };
    constexpr auto sizeYHalf{ sizeY * 0.5f };

    sf::VertexArray enemyModel;
    enemyModel.setPrimitiveType(sf::TriangleStrip);
    enemyModel.append(sf::Vertex({ -sizeXHalf, -sizeYHalf }, sf::Color::Red));
    enemyModel.append(sf::Vertex({ -sizeXHalf, sizeYHalf }, sf::Color::Red));
    enemyModel.append(sf::Vertex({ sizeXHalf, sizeYHalf }, sf::Color::Red));
    enemyModel.append(sf::Vertex({ sizeXHalf, -sizeYHalf }, sf::Color::Red));

    Dod::BufferUtils::constructBack(this->modelsContext.loadedModels);
    Dod::BufferUtils::get(this->modelsContext.loadedModels, 0) = enemyModel;

    Dod::BufferUtils::constructBack(this->modelsContext.modelIds);
    Dod::BufferUtils::get(this->modelsContext.modelIds, 0) = 1;

    Dod::BufferUtils::constructBack(this->modelsContext.modelNames);
    std::memcpy(Dod::BufferUtils::get(this->modelsContext.modelNames, 0).data(), "Enemy", sizeof("Enemy"));

}

bool Game::ExecutionBlock::Models::update(float dt)
{
    return true;
}

void Game::ExecutionBlock::Models::flushSharedLocalContexts()
{
    this->modelsContext.reset();
}
