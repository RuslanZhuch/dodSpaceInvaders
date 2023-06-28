#include "ModelsExecutor.h"

void Game::ExecutionBlock::Models::loadContext()
{
    this->modelsContext.load();
}

void Game::ExecutionBlock::Models::initiate()
{

    this->createEnemyModel();
    this->createEnemyBulletModel();
    this->createObstacleModel();
    this->createPlayerModel();

}

void Game::ExecutionBlock::Models::update(float dt)
{
    return;
}

void Game::ExecutionBlock::Models::flushSharedLocalContexts()
{
    this->modelsContext.reset();
}

void Game::ExecutionBlock::Models::createEnemyModel()
{

    constexpr auto sizeX{ 25.f };
    constexpr auto sizeY{ 30.f };
    constexpr auto sizeXHalf{ sizeX * 0.5f };
    constexpr auto sizeYHalf{ sizeY * 0.5f };

    constexpr auto sizeXHalfOutline{ sizeXHalf + 2.f };
    constexpr auto sizeYHalfOutline{ sizeYHalf + 2.f };

    sf::VertexArray enemyModel;
    enemyModel.setPrimitiveType(sf::TriangleStrip);
    enemyModel.append(sf::Vertex({ -sizeXHalfOutline, -sizeYHalfOutline }, sf::Color::Green));
    enemyModel.append(sf::Vertex({ sizeXHalfOutline, -sizeYHalfOutline }, sf::Color::Green));
    enemyModel.append(sf::Vertex({ -sizeXHalfOutline, sizeYHalfOutline }, sf::Color::Green));
    enemyModel.append(sf::Vertex({ sizeXHalfOutline, sizeYHalfOutline }, sf::Color::Green));
    enemyModel.append(sf::Vertex({ -sizeXHalf, -sizeYHalf }, sf::Color::Red));
    enemyModel.append(sf::Vertex({ sizeXHalf, -sizeYHalf }, sf::Color::Red));
    enemyModel.append(sf::Vertex({ -sizeXHalf, sizeYHalf }, sf::Color::Red));
    enemyModel.append(sf::Vertex({ sizeXHalf, sizeYHalf }, sf::Color::Red));

    Dod::BufferUtils::constructBack(this->modelsContext.loadedModels);
    Dod::BufferUtils::get(this->modelsContext.loadedModels, 0) = enemyModel;

    Dod::BufferUtils::constructBack(this->modelsContext.modelIds);
    Dod::BufferUtils::get(this->modelsContext.modelIds, 0) = 1;

    Dod::BufferUtils::constructBack(this->modelsContext.modelNames);
    std::memcpy(Dod::BufferUtils::get(this->modelsContext.modelNames, 0).data(), "Enemy", sizeof("Enemy"));

}

void Game::ExecutionBlock::Models::createEnemyBulletModel()
{

    /*
        constexpr auto length{ 15.f };
    renderer.drawRectangle({ x, y }, sf::Vector2f(6.f, length), sf::Color::Yellow, 1.f, sf::Color::Green);

    
    */

    constexpr auto sizeX{ 6.f };
    constexpr auto sizeY{ 15.f };
    constexpr auto sizeXHalf{ sizeX * 0.5f };
    constexpr auto sizeYHalf{ sizeY * 0.5f };

    constexpr auto sizeXHalfOutline{ sizeXHalf + 2.f };
    constexpr auto sizeYHalfOutline{ sizeYHalf + 2.f };

    sf::VertexArray enemyModel;
    enemyModel.setPrimitiveType(sf::TriangleStrip);
    enemyModel.append(sf::Vertex({ -sizeXHalfOutline, -sizeYHalfOutline }, sf::Color::Green));
    enemyModel.append(sf::Vertex({ sizeXHalfOutline, -sizeYHalfOutline }, sf::Color::Green));
    enemyModel.append(sf::Vertex({ -sizeXHalfOutline, sizeYHalfOutline }, sf::Color::Green));
    enemyModel.append(sf::Vertex({ sizeXHalfOutline, sizeYHalfOutline }, sf::Color::Green));
    enemyModel.append(sf::Vertex({ -sizeXHalf, -sizeYHalf }, sf::Color::Yellow));
    enemyModel.append(sf::Vertex({ sizeXHalf, -sizeYHalf }, sf::Color::Yellow));
    enemyModel.append(sf::Vertex({ -sizeXHalf, sizeYHalf }, sf::Color::Yellow));
    enemyModel.append(sf::Vertex({ sizeXHalf, sizeYHalf }, sf::Color::Yellow));

    Dod::BufferUtils::constructBack(this->modelsContext.loadedModels, enemyModel);
//    Dod::BufferUtils::get(this->modelsContext.loadedModels, 0) = enemyModel;

    Dod::BufferUtils::constructBack(this->modelsContext.modelIds, 2);
//    Dod::BufferUtils::get(this->modelsContext.modelIds, 0) = 2;

    Dod::BufferUtils::constructBack(this->modelsContext.modelNames);
    std::memcpy(Dod::BufferUtils::get(this->modelsContext.modelNames, 
        Dod::BufferUtils::getNumFilledElements(this->modelsContext.modelNames) - 1).data(), 
        "EnemyBullet", sizeof("EnemyBullet"));

}

void Game::ExecutionBlock::Models::createObstacleModel()
{

    constexpr auto sizeX{ 30.f };
    constexpr auto sizeY{ 30.f };
    constexpr auto sizeXHalf{ sizeX * 0.5f };
    constexpr auto sizeYHalf{ sizeY * 0.5f };

    constexpr auto sizeXHalfOutline{ sizeXHalf - 2.f };
    constexpr auto sizeYHalfOutline{ sizeYHalf - 2.f };

    sf::VertexArray enemyModel;
    enemyModel.setPrimitiveType(sf::TriangleStrip);
    enemyModel.append(sf::Vertex({ -sizeXHalf, -sizeYHalf }, sf::Color::Green));
    enemyModel.append(sf::Vertex({ sizeXHalf, -sizeYHalf }, sf::Color::Green));
    enemyModel.append(sf::Vertex({ -sizeXHalf, sizeYHalf }, sf::Color::Green));
    enemyModel.append(sf::Vertex({ sizeXHalf, sizeYHalf }, sf::Color::Green));
    enemyModel.append(sf::Vertex({ -sizeXHalfOutline, -sizeYHalfOutline }, sf::Color::White));
    enemyModel.append(sf::Vertex({ sizeXHalfOutline, -sizeYHalfOutline }, sf::Color::White));
    enemyModel.append(sf::Vertex({ -sizeXHalfOutline, sizeYHalfOutline }, sf::Color::White));
    enemyModel.append(sf::Vertex({ sizeXHalfOutline, sizeYHalfOutline }, sf::Color::White));

    Dod::BufferUtils::constructBack(this->modelsContext.loadedModels, enemyModel);

    Dod::BufferUtils::constructBack(this->modelsContext.modelIds, 3);

    Dod::BufferUtils::constructBack(this->modelsContext.modelNames);
    std::memcpy(Dod::BufferUtils::get(this->modelsContext.modelNames,
        Dod::BufferUtils::getNumFilledElements(this->modelsContext.modelNames) - 1).data(),
        "Obstacle", sizeof("Obstacle"));

}

void Game::ExecutionBlock::Models::createPlayerModel()
{

    /*
    
        const auto widthHalf{ width * 0.5f * strobe };
        const auto heightHalf{ height * 0.5f * strobe };
        const auto leftPoint{ position - sf::Vector2f(-widthHalf, -heightHalf) };
        const auto rightPoint{ position - sf::Vector2f(widthHalf, -heightHalf) };
        const auto topPoint{ position - sf::Vector2f(0.f, heightHalf) };

        const auto playerColor{ sf::Color(150, 200, 90) };
        renderer.drawLine(leftPoint, rightPoint, playerColor);
        renderer.drawLine(leftPoint, topPoint, playerColor);
        renderer.drawLine(rightPoint, topPoint, playerColor);
    
    
    */

    constexpr auto width{ 50.f };
    constexpr auto height{ 25.f };
    const auto widthHalf{ width * 0.5f };
    const auto heightHalf{ height * 0.5f };

    const auto leftPoint{ -sf::Vector2f(-widthHalf, -heightHalf) };
    const auto rightPoint{ -sf::Vector2f(widthHalf, -heightHalf) };
    const auto topPoint{ -sf::Vector2f(0.f, heightHalf) };

    sf::VertexArray enemyModel;
    enemyModel.setPrimitiveType(sf::LineStrip);
    const auto playerColor{ sf::Color(150, 200, 90) };
    enemyModel.append(sf::Vertex(leftPoint, playerColor));
    enemyModel.append(sf::Vertex(rightPoint, playerColor));
    enemyModel.append(sf::Vertex(topPoint, playerColor));
    enemyModel.append(sf::Vertex(leftPoint, playerColor));

    Dod::BufferUtils::constructBack(this->modelsContext.loadedModels, enemyModel);

    Dod::BufferUtils::constructBack(this->modelsContext.modelIds, 4);

    Dod::BufferUtils::constructBack(this->modelsContext.modelNames);
    std::memcpy(Dod::BufferUtils::get(this->modelsContext.modelNames,
        Dod::BufferUtils::getNumFilledElements(this->modelsContext.modelNames) - 1).data(),
        "Player", sizeof("Player"));

}
