#include "pch.h"
#include "MockInstantRenderer.h"

#include "gameTests/GameTestsContext.h"

//namespace Mock::Renderer
//{
//	sf::RenderWindow* gWindow{};
//	sf::Vector2f gPosition{};
//	sf::Vector2f gPosition1{};
//	sf::Vector2f gPosition2{};
//	float gRadius{};
//	sf::Vector2f gScale{};
//	sf::Color gColor{};
//	sf::Color gFillColor{};
//	float gOutlineWidth{};
//	sf::Color gOutlineColor{};
//	float gX{};
//	float gX1{};
//	float gX2{};
//	float gY{};
//	float gY1{};
//	float gY2{};
//	float gWidth{};
//}

namespace Renderer::Instant
{

	void Renderer::Instant::drawRectangle(
		sf::RenderWindow& window,
		const sf::Vector2f& position,
		const sf::Vector2f& size,
		const sf::Color& outlineColor,
		float outlineWidth,
		const sf::Color& fillColor
	)
	{

		auto& ctx{ GameTests::ctx.ctxs.emplace_back() };

		ctx.instantRenderer.window = &window;
		ctx.instantRenderer.position = position;
		ctx.instantRenderer.scale = size;
		ctx.instantRenderer.outlineColor = outlineColor;
		ctx.instantRenderer.outlineWidth = outlineWidth;
		ctx.instantRenderer.fillColor = fillColor;

	}

	void Renderer::Instant::drawCircle(
		sf::RenderWindow& window,
		const sf::Vector2f& position,
		const float radius,
		const sf::Color& outlineColor,
		float outlineWidth,
		const sf::Color& fillColor
	)
	{

		auto& ctx{ GameTests::ctx.ctxs.emplace_back() };

		ctx.instantRenderer.window = &window;
		ctx.instantRenderer.position = position;
		ctx.instantRenderer.radius = radius;
		ctx.instantRenderer.outlineColor = outlineColor;
		ctx.instantRenderer.outlineWidth = outlineWidth;
		ctx.instantRenderer.fillColor = fillColor;

	}

	void Renderer::Instant::drawLine(
		sf::RenderWindow& window,
		const sf::Vector2f& position1,
		const sf::Vector2f& position2,
		const sf::Color& color
	)
	{

		auto& ctx{ GameTests::ctx.ctxs.emplace_back() };

		ctx.instantRenderer.window = &window;
		ctx.instantRenderer.position1 = position1;
		ctx.instantRenderer.position2 = position2;
		ctx.instantRenderer.color = color;

	}

};

void Renderer::Instant::drawLineThickHor(
	sf::RenderWindow& window,
	float x1,
	float x2,
	float y,
	const sf::Color& color,
	float width
)
{

	auto& ctx{ GameTests::ctx.ctxs.emplace_back() };

	ctx.instantRenderer.window = &window;
	ctx.instantRenderer.x1 = x1;
	ctx.instantRenderer.x2 = x2;
	ctx.instantRenderer.y = y;
	ctx.instantRenderer.color = color;
	ctx.instantRenderer.width = width;

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

	auto& ctx{ GameTests::ctx.ctxs.emplace_back() };

	ctx.instantRenderer.window = &window;
	ctx.instantRenderer.x = x;
	ctx.instantRenderer.y1 = y1;
	ctx.instantRenderer.y2 = y2;
	ctx.instantRenderer.color = color;
	ctx.instantRenderer.width = width;

}
