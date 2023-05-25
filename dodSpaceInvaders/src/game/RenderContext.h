#pragma once

#include <dod/BufferUtils.h>

#include "GameRender.h"

namespace Game::Context::Render
{

	struct RenderBuffer
	{
		Dod::DBBuffer<int32_t> idsToRender;
	};

};
