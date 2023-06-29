#include "ModelsExecutor.h"

void Game::ExecutionBlock::Models::loadContext()
{
    this->modelsContext.load();
    this->commonParameters.load();
    this->sceneParameters.load();
    this->playerDimentions.load();
}

void Game::ExecutionBlock::Models::initiate()
{

    this->createEnemyModel();
    this->createEnemyBulletModel();
    this->createObstacleModel();
    this->createPlayerModel();
    this->createSceneModel();

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

    Dod::BufferUtils::constructBack(this->modelsContext.modelIds, 2);

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

    const auto width{ this->playerDimentions.width };
    const auto height{ this->playerDimentions.height };
    const auto widthHalf{ width };
    const auto heightHalf{ height };

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

void Game::ExecutionBlock::Models::createSceneModel()
{

    const auto padding{ this->sceneParameters.padding };

    const auto left{ padding };
    const auto right{ this->commonParameters.width - padding };
    const auto top{ padding };
    const auto bottom{ this->commonParameters.height - padding };

    sf::VertexArray sceneModel;
    sceneModel.setPrimitiveType(sf::LineStrip);
    const auto color{ sf::Color(150, 200, 90) };
    sceneModel.append(sf::Vertex({ left, top }, color));
    sceneModel.append(sf::Vertex({ right, top }, color));
    sceneModel.append(sf::Vertex({ right, bottom }, color));
    sceneModel.append(sf::Vertex({ left, bottom }, color));
    sceneModel.append(sf::Vertex({ left, top }, color));

    Dod::BufferUtils::constructBack(this->modelsContext.loadedModels, sceneModel);

    Dod::BufferUtils::constructBack(this->modelsContext.modelIds, 5);

    Dod::BufferUtils::constructBack(this->modelsContext.modelNames);
    std::memcpy(Dod::BufferUtils::get(this->modelsContext.modelNames,
        Dod::BufferUtils::getNumFilledElements(this->modelsContext.modelNames) - 1).data(),
        "Scene", sizeof("Scene"));

}
