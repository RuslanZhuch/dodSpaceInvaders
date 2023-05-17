#include "pch.h"

#include <game/Renderer.h>

#include <game/SceneRenderer.cpp>

#include "Mocks/MockInstantRenderer.h"
#include "gameTests/GameTestsContext.h"

TEST(SceneRender, DrawField)
{

	Game::GameRenderer renderer(100, 200, "Test name");

	Game::SceneRenderer::drawField(renderer);

	const auto& ctx{ GameTests::ctx.ctxs.back() };

	EXPECT_EQ(ctx.instantRenderer.position.x, 120 * 0.5f);
	EXPECT_EQ(ctx.instantRenderer.position.y, 200 * 0.5f);
	EXPECT_EQ(ctx.instantRenderer.scale.x, 120 * 0.95f);
	EXPECT_EQ(ctx.instantRenderer.scale.y, 200 * 0.95f);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.r, 200);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.g, 100);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.b, 50);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.a, 200);
	EXPECT_EQ(ctx.instantRenderer.outlineWidth, 2.f);
	EXPECT_EQ(ctx.instantRenderer.fillColor.r, 0);
	EXPECT_EQ(ctx.instantRenderer.fillColor.g, 0);
	EXPECT_EQ(ctx.instantRenderer.fillColor.b, 0);
	EXPECT_EQ(ctx.instantRenderer.fillColor.a, 0);

}

TEST(SceneRender, DrawPlayer)
{

	Game::GameRenderer renderer(100, 200, "Test name");

	Game::SceneRenderer::drawPlayer(renderer, {10.f, 20.f}, 50.f, 25.f, 1);

	const auto totalCtxs{ GameTests::ctx.ctxs.size() };
	EXPECT_GE(totalCtxs, 3);
	if (totalCtxs < 3)
		return;

	{
		const auto& ctx{ GameTests::ctx.ctxs.at(totalCtxs - 3) };

		EXPECT_EQ(ctx.instantRenderer.position1.x, 10.f + 25.f);
		EXPECT_EQ(ctx.instantRenderer.position1.y, 20.f + 25.f * 0.5f);
		EXPECT_EQ(ctx.instantRenderer.position2.x, 10.f - 25.f);
		EXPECT_EQ(ctx.instantRenderer.position2.y, 20.f + 25.f * 0.5f);
		EXPECT_EQ(ctx.instantRenderer.color.r, 150);
		EXPECT_EQ(ctx.instantRenderer.color.g, 200);
		EXPECT_EQ(ctx.instantRenderer.color.b, 90);
	}
	{
		const auto& ctx{ GameTests::ctx.ctxs.at(totalCtxs - 2) };

		EXPECT_EQ(ctx.instantRenderer.position1.x, 10.f + 25.f);
		EXPECT_EQ(ctx.instantRenderer.position1.y, 20.f + 25.f * 0.5f);
		EXPECT_EQ(ctx.instantRenderer.position2.x, 10.f);
		EXPECT_EQ(ctx.instantRenderer.position2.y, 20.f - 25.f * 0.5f);
		EXPECT_EQ(ctx.instantRenderer.color.r, 150);
		EXPECT_EQ(ctx.instantRenderer.color.g, 200);
		EXPECT_EQ(ctx.instantRenderer.color.b, 90);
	}
	{
		const auto& ctx{ GameTests::ctx.ctxs.at(totalCtxs - 1) };

		EXPECT_EQ(ctx.instantRenderer.position1.x, 10.f - 25.f);
		EXPECT_EQ(ctx.instantRenderer.position1.y, 20.f + 25.f * 0.5f);
		EXPECT_EQ(ctx.instantRenderer.position2.x, 10.f);
		EXPECT_EQ(ctx.instantRenderer.position2.y, 20.f - 25.f * 0.5f);
		EXPECT_EQ(ctx.instantRenderer.color.r, 150);
		EXPECT_EQ(ctx.instantRenderer.color.g, 200);
		EXPECT_EQ(ctx.instantRenderer.color.b, 90);
	}

}

TEST(SceneRender, DrawObstacle)
{

	Game::GameRenderer renderer(100, 200, "Test name");

	Game::SceneRenderer::drawObstacle(renderer, 150.f, 250.f);

	const auto& ctx{ GameTests::ctx.ctxs.back() };

	EXPECT_EQ(ctx.instantRenderer.position.x, 150.f);
	EXPECT_EQ(ctx.instantRenderer.position.y, 250);
	EXPECT_EQ(ctx.instantRenderer.scale.x, 30.f);
	EXPECT_EQ(ctx.instantRenderer.scale.y, 30.f);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.r, 0);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.g, 255);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.b, 0);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.a, 255);
	EXPECT_EQ(ctx.instantRenderer.outlineWidth, 1.f);
	EXPECT_EQ(ctx.instantRenderer.fillColor.r, 255);
	EXPECT_EQ(ctx.instantRenderer.fillColor.g, 255);
	EXPECT_EQ(ctx.instantRenderer.fillColor.b, 255);
	EXPECT_EQ(ctx.instantRenderer.fillColor.a, 255);

}

TEST(SceneRender, DrawBullet)
{

	Game::GameRenderer renderer(100, 200, "Test name");

	Game::SceneRenderer::drawBullet(renderer, 350.f, 450.f);

	const auto& ctx{ GameTests::ctx.ctxs.back() };

	EXPECT_EQ(ctx.instantRenderer.position.x, 350.f);
	EXPECT_EQ(ctx.instantRenderer.position.y, 450);
	EXPECT_EQ(ctx.instantRenderer.scale.x, 6.f);
	EXPECT_EQ(ctx.instantRenderer.scale.y, 15.f);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.r, 255);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.g, 255);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.b, 0);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.a, 255);
	EXPECT_EQ(ctx.instantRenderer.outlineWidth, 1.f);
	EXPECT_EQ(ctx.instantRenderer.fillColor.r, 0);
	EXPECT_EQ(ctx.instantRenderer.fillColor.g, 255);
	EXPECT_EQ(ctx.instantRenderer.fillColor.b, 0);
	EXPECT_EQ(ctx.instantRenderer.fillColor.a, 255);

}

TEST(SceneRender, DrawEnemy)
{

	/*
	
	
    constexpr auto sizeX{ 25.f };
    constexpr auto sizeY{ 30.f };
    Renderer::Instant::drawRectangle(window, position, sf::Vector2f(sizeX, sizeY), sf::Color::Blue, 1.f, sf::Color::Red);
	
	*/

	Game::GameRenderer renderer(100, 200, "Test name");

	Game::SceneRenderer::drawEnemy(renderer, 550.f, 650.f);

	const auto& ctx{ GameTests::ctx.ctxs.back() };

	EXPECT_EQ(ctx.instantRenderer.position.x, 550.f);
	EXPECT_EQ(ctx.instantRenderer.position.y, 650);
	EXPECT_EQ(ctx.instantRenderer.scale.x, 25.f);
	EXPECT_EQ(ctx.instantRenderer.scale.y, 30.f);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.r, 0);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.g, 0);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.b, 255);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.a, 255);
	EXPECT_EQ(ctx.instantRenderer.outlineWidth, 1.f);
	EXPECT_EQ(ctx.instantRenderer.fillColor.r, 255);
	EXPECT_EQ(ctx.instantRenderer.fillColor.g, 0);
	EXPECT_EQ(ctx.instantRenderer.fillColor.b, 0);
	EXPECT_EQ(ctx.instantRenderer.fillColor.a, 255);

}
