#include "Renderer.h"

void Game::GameRenderer::drawRectangle(
	const sf::Vector2f& position, 
	const sf::Vector2f& size, 
	const sf::Color& outlineColor, 
	float outlineWidth, 
	const sf::Color& fillColor
) noexcept
{
	Renderer::Instant::drawRectangle(
		this->window,
		position,
		size,
		outlineColor,
		outlineWidth,
		fillColor
	);
}

void Game::GameRenderer::drawCircle(
	const sf::Vector2f& position, 
	const float radius, 
	const sf::Color& outlineColor, 
	float outlineWidth, 
	const sf::Color& fillColor
) noexcept
{
	Renderer::Instant::drawCircle(
		this->window,
		position,
		radius,
		outlineColor,
		outlineWidth,
		fillColor
	);
}

void Game::GameRenderer::drawLine(
	const sf::Vector2f& position1, 
	const sf::Vector2f& position2, 
	const sf::Color& color
) noexcept
{
	Renderer::Instant::drawLine(
		this->window,
		position1,
		position2,
		color
	);
}

void Game::GameRenderer::drawLineThickHor(
	float x1, 
	float x2, 
	float y, 
	const sf::Color& color, 
	float width
) noexcept
{
	Renderer::Instant::drawLineThickHor(
		this->window,
		x1,
		x2,
		y,
		color,
		width
	);
}

void Game::GameRenderer::drawLineThickVert(
	float x, 
	float y1, 
	float y2, 
	const sf::Color& color, 
	float width
) noexcept
{
	Renderer::Instant::drawLineThickVert(
		this->window,
		x,
		y1,
		y2,
		color,
		width
	);
}
