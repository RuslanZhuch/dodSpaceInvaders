#pragma once

#include <mutex>
#include <vector>
#include <memory>

#include <internal/render/Renderer.h>

namespace GameTests
{
	struct Context
	{

		struct InstantRenderer
		{
			sf::RenderWindow* window{};
			sf::Vector2f position{};
			sf::Vector2f position1{};
			sf::Vector2f position2{};
			float radius{};
			sf::Vector2f scale{};
			sf::Color color{};
			sf::Color fillColor{};
			float outlineWidth{};
			sf::Color outlineColor{};
			float x{};
			float x1{};
			float x2{};
			float y{};
			float y1{};
			float y2{};
			float width{};
		};

		InstantRenderer instantRenderer;

	};

	struct ContextHolder
	{
		std::vector<Context> ctxs;
	};

	extern ContextHolder ctx;

}
