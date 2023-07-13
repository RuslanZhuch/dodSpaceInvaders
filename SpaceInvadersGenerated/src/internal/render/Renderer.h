#pragma once
#include "instantRenderer.h"

#include <memory>

namespace Game
{

	class GameRenderer
	{
	public:
		explicit GameRenderer(int width, int height, std::string_view windowName) noexcept
			:
			window(sf::VideoMode(width, height), windowName.data())
		{}
		
		[[nodiscard]] auto& getWindow() { return this->window; }

		void drawRectangle(
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Color& outlineColor,
			float outlineWidth = 4.f,
			const sf::Color& fillColor = sf::Color::Transparent
		) noexcept;

		void drawCircle(
			const sf::Vector2f& position,
			const float radius,
			const sf::Color& outlineColor,
			float outlineWidth = 4.f,
			const sf::Color& fillColor = sf::Color::Transparent
		) noexcept;

		void drawLine(
			const sf::Vector2f& position1,
			const sf::Vector2f& position2,
			const sf::Color& color
		) noexcept;

		void drawLineThickHor(
			float x1,
			float x2,
			float y,
			const sf::Color& color,
			float width
		) noexcept;

		void drawLineThickVert(
			float x,
			float y1,
			float y2,
			const sf::Color& color,
			float width
		) noexcept;

	private:
		sf::RenderWindow window;

	};

	using GameRendererPtr = std::unique_ptr<GameRenderer>;

};
