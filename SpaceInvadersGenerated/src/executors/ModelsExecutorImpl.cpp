#include "ModelsExecutor.h"

#include <dod/BufferUtils.h>

static void createEnemyModel(Game::Context::ModelsToRender::Data& modelsContext)
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

    Dod::BufferUtils::constructBack(modelsContext.loadedModels);
    Dod::BufferUtils::get(modelsContext.loadedModels, 0) = enemyModel;

    Dod::BufferUtils::constructBack(modelsContext.modelIds);
    Dod::BufferUtils::get(modelsContext.modelIds, 0) = 1;

//    Dod::BufferUtils::constructBack(modelsContext.modelNames);
//    std::memcpy(Dod::BufferUtils::get(modelsContext.modelNames, 0).data(), "Enemy", sizeof("Enemy"));

}

static void createEnemyBulletModel(Game::Context::ModelsToRender::Data& modelsContext)
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

    Dod::BufferUtils::constructBack(modelsContext.loadedModels, enemyModel);

    Dod::BufferUtils::constructBack(modelsContext.modelIds, 2);

//    Dod::BufferUtils::constructBack(this->modelsContext.modelNames);
//    std::memcpy(Dod::BufferUtils::get(this->modelsContext.modelNames,
//        Dod::BufferUtils::getNumFilledElements(this->modelsContext.modelNames) - 1).data(),
//        "EnemyBullet", sizeof("EnemyBullet"));

}

static void createObstacleModel(Game::Context::ModelsToRender::Data& modelsContext)
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

    Dod::BufferUtils::constructBack(modelsContext.loadedModels, enemyModel);

    Dod::BufferUtils::constructBack(modelsContext.modelIds, 3);

//    Dod::BufferUtils::constructBack(this->modelsContext.modelNames);
//    std::memcpy(Dod::BufferUtils::get(this->modelsContext.modelNames,
//        Dod::BufferUtils::getNumFilledElements(this->modelsContext.modelNames) - 1).data(),
//        "Obstacle", sizeof("Obstacle"));

}

static void createPlayerModel(
    Game::Context::PlayerDimentions::Data& playerDimentions,
    Game::Context::ModelsToRender::Data& modelsContext
)
{

    const auto width{ playerDimentions.width };
    const auto height{ playerDimentions.height };
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

    Dod::BufferUtils::constructBack(modelsContext.loadedModels, enemyModel);

    Dod::BufferUtils::constructBack(modelsContext.modelIds, 4);

//    Dod::BufferUtils::constructBack(modelsContext.modelNames);
//    std::memcpy(Dod::BufferUtils::get(modelsContext.modelNames,
//        Dod::BufferUtils::getNumFilledElements(modelsContext.modelNames) - 1).data(),
//        "Player", sizeof("Player"));

}

static void createSceneModel(
    const Game::Context::Common::Data& commonParameters,
    const Game::Context::Scene::Data& sceneParameters,
    Game::Context::ModelsToRender::Data& modelsContext
)
{

    const auto padding{ sceneParameters.padding };

    const auto left{ padding };
    const auto right{ commonParameters.width - padding };
    const auto top{ padding };
    const auto bottom{ commonParameters.height - padding };

    sf::VertexArray sceneModel;
    sceneModel.setPrimitiveType(sf::LineStrip);
    const auto color{ sf::Color(150, 200, 90) };
    sceneModel.append(sf::Vertex({ left, top }, color));
    sceneModel.append(sf::Vertex({ right, top }, color));
    sceneModel.append(sf::Vertex({ right, bottom }, color));
    sceneModel.append(sf::Vertex({ left, bottom }, color));
    sceneModel.append(sf::Vertex({ left, top }, color));

    Dod::BufferUtils::constructBack(modelsContext.loadedModels, sceneModel);

    Dod::BufferUtils::constructBack(modelsContext.modelIds, 5);

//    Dod::BufferUtils::constructBack(this->modelsContext.modelNames);
//    std::memcpy(Dod::BufferUtils::get(this->modelsContext.modelNames,
//        Dod::BufferUtils::getNumFilledElements(this->modelsContext.modelNames) - 1).data(),
//        "Scene", sizeof("Scene"));

}

namespace Game::ExecutionBlock
{

    void Models::initImpl() noexcept
    {
        createEnemyModel(this->modelsContext);
        createEnemyBulletModel(this->modelsContext);
        createObstacleModel(this->modelsContext);
        createPlayerModel(this->playerParamsContext, this->modelsContext);
        createSceneModel(this->windowParamsContext, this->sceneParamsContext, this->modelsContext);
    }

    void Models::updateImpl(float dt) noexcept
    {

    }

}
