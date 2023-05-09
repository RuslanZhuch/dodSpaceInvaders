#pragma once

#include <dod/BufferUtils.h>

namespace Game::Core::Collision
{

	void pointsAreaIntersection(
		Dod::DBBuffer<int32_t>& intersections,
		const Dod::ImBuffer<float>& pointsCenter,
		const float areaCenter,
		const float areaWidth
	) noexcept;

	void pointsAreasIntersection(
		Dod::DBBuffer<uint64_t>& collisions, 
		const Dod::ImBuffer<float>& lefts, 
		const Dod::ImBuffer<float>& rights,
		float maxRadius
	) noexcept;	
	
	void pointsSquareIntersection(
		Dod::DBBuffer<int32_t>& intersections,
		const Dod::ImBuffer<float>& xPointsCoords, 
		const Dod::ImBuffer<float>& yPointsCoords,
		float squareX,
		float squareY,
		float width,
		float height
	) noexcept;

	void pointsSquaresIntersection(
		Dod::DBBuffer<uint64_t>& intersections, 
		const Dod::ImBuffer<float>& xPointsCoords, 
		const Dod::ImBuffer<float>& yPointsCoords, 
		const Dod::ImBuffer<float>& xSquaresCoords,
		const Dod::ImBuffer<float>& ySquaresCoords,
		float widths,
		float heights
	) noexcept;

	void pointsPlaneIntersection(
		Dod::DBBuffer<int32_t>& intersections,
		const Dod::ImBuffer<float>& yPointsCoords,
		float planeY,
		float direction
	) noexcept;

};
