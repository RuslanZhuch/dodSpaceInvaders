#include "instantRenderer.h"

void Renderer::Instant::drawRectangle(
	sf::RenderWindow& window,
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	const sf::Color& outlineColor,
	float outlineWidth,
	const sf::Color& fillColor
)
{

	sf::RectangleShape shape(size);
	shape.setOrigin({size.x * 0.5f, size.y * 0.5f});
	shape.setPosition(position);
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineWidth);
	
	window.draw(shape);

}

void Renderer::Instant::drawCircle(
	sf::RenderWindow& window, 
	const sf::Vector2f& position, 
	float radius, 
	const sf::Color& outlineColor, 
	float outlineWidth, 
	const sf::Color& fillColor
)
{

	sf::CircleShape shape(radius);
	shape.setOrigin({ radius, radius });
	shape.setPosition(position);
	shape.setFillColor(fillColor);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineWidth);

	window.draw(shape);

}

void Renderer::Instant::drawLine(
	sf::RenderWindow& window,
	const sf::Vector2f& position1,
	const sf::Vector2f& position2,
	const sf::Color& color
)
{

	sf::VertexArray line(sf::LineStrip, 2);
	line[0].color = color;
	line[0].position = position1;
	line[1].color = color;
	line[1].position = position2;

	window.draw(line);

}

void Renderer::Instant::drawLineThickHor(
	sf::RenderWindow& window, 
	float x1,
	float x2,
	float y,
	const sf::Color& color, 
	float width
)
{
	const auto position1{ sf::Vector2f(x1, y) };
	const auto position2{ sf::Vector2f(x2, y) };
	const auto centerPosition{ (position1 + position2) / 2.f };
	const auto len = std::abs(x1 - x2);
	drawRectangle(window, centerPosition, { len, width }, sf::Color::Black, 0.f, color);
}

void Renderer::Instant::drawLineThickVert(
	sf::RenderWindow& window,
	float x,
	float y1,
	float y2,
	const sf::Color& color,
	float width
)
{
	const auto position1{ sf::Vector2f(x, y1) };
	const auto position2{ sf::Vector2f(x, y2) };
	const auto centerPosition{ (position1 + position2) / 2.f };
	const auto len = std::abs(y1 - y2);
	drawRectangle(window, centerPosition, { width, len }, sf::Color::Black, 0.f, color);
}
