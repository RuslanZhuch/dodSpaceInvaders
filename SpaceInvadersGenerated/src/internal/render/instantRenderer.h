#pragma once

#pragma warning(push)

#pragma warning(disable : 4668)
#pragma warning(disable : 4820)
#pragma warning(disable : 4625)
#pragma warning(disable : 4514)
#pragma warning(disable : 4626)
#pragma warning(disable : 4365)

#include <SFML/Graphics.hpp>

#pragma warning(pop)

namespace Renderer::Instant
{

	void drawRectangle(
		sf::RenderWindow& window,
		const sf::Vector2f& position,
		const sf::Vector2f& size,
		const sf::Color& outlineColor,
		float outlineWidth = 4.f,
		const sf::Color& fillColor = sf::Color::Transparent
	);

	void drawCircle(
		sf::RenderWindow& window,
		const sf::Vector2f& position,
		float radius,
		const sf::Color& outlineColor,
		float outlineWidth = 4.f,
		const sf::Color& fillColor = sf::Color::Transparent
	);

	void drawLine(
		sf::RenderWindow& window, 
		const sf::Vector2f& position1, 
		const sf::Vector2f& position2, 
		const sf::Color& color
	);

	void drawLineThickHor(
		sf::RenderWindow& window,
		float x1,
		float x2,
		float y,
		const sf::Color& color,
		float width
	);

	void drawLineThickVert(
		sf::RenderWindow& window,
		float x,
		float y1,
		float y2,
		const sf::Color& color,
		float width
	);

};