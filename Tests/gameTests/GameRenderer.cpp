#include "pch.h"

#include <game/Renderer.h>
#include <game/Renderer.cpp>

#include "Mocks/MockInstantRenderer.h"
#include "gameTests/GameTestsContext.h"

TEST(GameRenderer, DrawRectangle)
{

	Game::GameRenderer renderer(100, 200, "Test name");

	renderer.drawRectangle(
		{1.f, 2.f},
		{3.f, 4.f},
		{5, 6, 7},
		8.f,
		{9, 10, 11}
	);

	const auto& ctx{ GameTests::ctx.ctxs.back()};

	EXPECT_NE(ctx.instantRenderer.window, nullptr);

	EXPECT_EQ(ctx.instantRenderer.position.x, 1.f);
	EXPECT_EQ(ctx.instantRenderer.position.y, 2.f);
	EXPECT_EQ(ctx.instantRenderer.scale.x, 3.f);
	EXPECT_EQ(ctx.instantRenderer.scale.y, 4.f);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.r, 5);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.g, 6);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.b, 7);
	EXPECT_EQ(ctx.instantRenderer.outlineWidth, 8.f);
	EXPECT_EQ(ctx.instantRenderer.fillColor.r, 9);
	EXPECT_EQ(ctx.instantRenderer.fillColor.g, 10);
	EXPECT_EQ(ctx.instantRenderer.fillColor.b, 11);

}

TEST(GameRenderer, drawCircle)
{

	Game::GameRenderer renderer(100, 200, "Test name");

	renderer.drawCircle(
		{ 101.f, 102.f },
		103.f,
		{ 105, 106, 107 },
		108.f,
		{ 109, 110, 111 }
	);

	const auto& ctx{ GameTests::ctx.ctxs.back() };

	EXPECT_NE(ctx.instantRenderer.window, nullptr);

	EXPECT_EQ(ctx.instantRenderer.position.x, 101.f);
	EXPECT_EQ(ctx.instantRenderer.position.y, 102.f);
	EXPECT_EQ(ctx.instantRenderer.radius, 103.f);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.r, 105);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.g, 106);
	EXPECT_EQ(ctx.instantRenderer.outlineColor.b, 107);
	EXPECT_EQ(ctx.instantRenderer.outlineWidth, 108.f);
	EXPECT_EQ(ctx.instantRenderer.fillColor.r, 109);
	EXPECT_EQ(ctx.instantRenderer.fillColor.g, 110);
	EXPECT_EQ(ctx.instantRenderer.fillColor.b, 111);

}

TEST(GameRenderer, drawLine)
{

	Game::GameRenderer renderer(100, 200, "Test name");

	renderer.drawLine(
		{ 201.f, 202.f },
		{ 203.f, 204.f },
		{ 205, 206, 207 }
	);

	const auto& ctx{ GameTests::ctx.ctxs.back() };

	EXPECT_NE(ctx.instantRenderer.window, nullptr);

	EXPECT_EQ(ctx.instantRenderer.position1.x, 201.f);
	EXPECT_EQ(ctx.instantRenderer.position1.y, 202.f);
	EXPECT_EQ(ctx.instantRenderer.position2.x, 203.f);
	EXPECT_EQ(ctx.instantRenderer.position2.y, 204.f);
	EXPECT_EQ(ctx.instantRenderer.color.r, 205);
	EXPECT_EQ(ctx.instantRenderer.color.g, 206);
	EXPECT_EQ(ctx.instantRenderer.color.b, 207);

}

TEST(GameRenderer, drawLineThickHor)
{

	Game::GameRenderer renderer(100, 200, "Test name");

	renderer.drawLineThickHor(
		301.f,
		302.f,
		303.f,
		{34, 35, 36},
		307.f
	);

	const auto& ctx{ GameTests::ctx.ctxs.back() };

	EXPECT_NE(ctx.instantRenderer.window, nullptr);

	EXPECT_EQ(ctx.instantRenderer.x1, 301.f);
	EXPECT_EQ(ctx.instantRenderer.x2, 302);
	EXPECT_EQ(ctx.instantRenderer.y, 303.f);
	EXPECT_EQ(ctx.instantRenderer.color.r, 34);
	EXPECT_EQ(ctx.instantRenderer.color.g, 35);
	EXPECT_EQ(ctx.instantRenderer.color.b, 36);
	EXPECT_EQ(ctx.instantRenderer.width, 307.f);

}

TEST(GameRenderer, drawLineThickVert)
{

	Game::GameRenderer renderer(100, 200, "Test name");

	renderer.drawLineThickVert(
		401.f,
		402.f,
		403.f,
		{ 44, 45, 46 },
		407.f
	);

	const auto& ctx{ GameTests::ctx.ctxs.back() };

	EXPECT_NE(ctx.instantRenderer.window, nullptr);

	EXPECT_EQ(ctx.instantRenderer.x, 401.f);
	EXPECT_EQ(ctx.instantRenderer.y1, 402);
	EXPECT_EQ(ctx.instantRenderer.y2, 403.f);
	EXPECT_EQ(ctx.instantRenderer.color.r, 44);
	EXPECT_EQ(ctx.instantRenderer.color.g, 45);
	EXPECT_EQ(ctx.instantRenderer.color.b, 46);
	EXPECT_EQ(ctx.instantRenderer.width, 407.f);

}
