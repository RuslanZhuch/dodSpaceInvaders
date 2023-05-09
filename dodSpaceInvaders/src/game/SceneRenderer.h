#pragma once

#include "Renderer.h"

namespace Game::SceneRenderer
{

	void drawField(GameRenderer& renderer) noexcept;
	void drawPlayer(GameRenderer& renderer, const sf::Vector2f& position, bool strobe) noexcept;
	void drawObstacle(GameRenderer& renderer, float x, float y) noexcept;
	void drawBullet(GameRenderer& renderer, float x, float y) noexcept;
	void drawEnemy(GameRenderer& renderer, float x, float y) noexcept;

}
