#include "Scene.h"

void Game::Scene::drawField(GameRenderer& renderer) noexcept
{

    const auto windowSize{ renderer.getWindow().getSize() };
    const auto centerPoint{ sf::Vector2f(windowSize) * 0.5f };
    constexpr auto filedSizeCoeff{ 0.95f };
    const auto fieldSize{ sf::Vector2f(windowSize) * filedSizeCoeff };

    const auto color{ sf::Color(200, 100, 50, 200) };

    renderer.drawRectangle(centerPoint, fieldSize, color, 2.f);

}

void Game::Scene::drawPlayer(GameRenderer& renderer, const sf::Vector2f& position, bool strobe) noexcept
{

    const auto radius{ 25.f * strobe };
    const auto leftPoint{ position - sf::Vector2f(-radius, -radius * 0.5f) };
    const auto rightPoint{ position - sf::Vector2f(radius, -radius * 0.5f) };
    const auto topPoint{ position - sf::Vector2f(0.f, radius * 0.5f) };

    const auto playerColor{ sf::Color(150, 200, 90) };
    renderer.drawLine(leftPoint, rightPoint, playerColor);
    renderer.drawLine(leftPoint, topPoint, playerColor);
    renderer.drawLine(rightPoint, topPoint, playerColor);

}

void Game::Scene::drawObstacle(GameRenderer& renderer, float x, float y) noexcept
{

    constexpr auto length{ 30.f };
    renderer.drawRectangle({ x, y }, sf::Vector2f(length, length), sf::Color::Green, 1.f, sf::Color::White);

}

void Game::Scene::drawBullet(GameRenderer& renderer, float x, float y) noexcept
{

    constexpr auto length{ 15.f };
    renderer.drawRectangle({ x, y }, sf::Vector2f(6.f, length), sf::Color::Yellow, 1.f, sf::Color::Green);

}

void Game::Scene::drawEnemy(GameRenderer& renderer, float x, float y) noexcept
{

    constexpr auto sizeX{ 25.f };
    constexpr auto sizeY{ 30.f };
    renderer.drawRectangle({ x, y }, sf::Vector2f(sizeX, sizeY), sf::Color::Blue, 1.f, sf::Color::Red);

}
