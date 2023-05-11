#pragma once

#include <dod/BufferUtils.h>

namespace Game::Core::Collision
{

	void pointsAreaIntersection(
		Dod::DBBuffer<int32_t>& intersections,
		Dod::ImBuffer<float> pointsCenter,
		const float areaCenter,
		const float areaWidth
	) noexcept;

	void pointsAreasIntersection(
		Dod::DBBuffer<uint64_t>& collisions, 
		Dod::ImBuffer<float> lefts, 
		Dod::ImBuffer<float> rights,
		float maxRadius
	) noexcept;	
	
	void pointsSquareIntersection(
		Dod::DBBuffer<int32_t>& intersections,
		Dod::ImBuffer<float> xPointsCoords, 
		Dod::ImBuffer<float> yPointsCoords,
		float squareX,
		float squareY,
		float width,
		float height
	) noexcept;

	void pointsSquaresIntersection(
		Dod::DBBuffer<uint64_t>& intersections, 
		Dod::ImBuffer<float> xPointsCoords, 
		Dod::ImBuffer<float> yPointsCoords, 
		Dod::ImBuffer<float> xSquaresCoords,
		Dod::ImBuffer<float> ySquaresCoords,
		float widths,
		float heights
	) noexcept;

	void pointsPlaneIntersection(
		Dod::DBBuffer<int32_t>& intersections,
		Dod::ImBuffer<float> yPointsCoords,
		float planeY,
		float direction
	) noexcept;

};
