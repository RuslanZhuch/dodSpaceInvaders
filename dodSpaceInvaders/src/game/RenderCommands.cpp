#include "RenderCommnads.h"

#include <engine/contextUtils.h>

namespace Game::Context::RenderCommands
{

	void Data::load() noexcept
	{

		const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/renderCommands.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 3) };

		if (!inputDataOpt.has_value())
			return;

		const auto& loadingDataArray{ inputDataOpt.value() };

		const auto xCoordsCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 0) };
		const auto yCoordsCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 1) };
		const auto modelsMetaCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 2) };

		int32_t needBytes{};
		needBytes += xCoordsCapacityBytes;
		needBytes += yCoordsCapacityBytes;
		needBytes += modelsMetaCapacityBytes;

		this->memory.allocate(needBytes);
		int32_t header{};

		Engine::ContextUtils::loadBuffer(this->xCoords, xCoordsCapacityBytes, this->memory, header);
		Engine::ContextUtils::loadBuffer(this->yCoords, yCoordsCapacityBytes, this->memory, header);
		Engine::ContextUtils::loadBuffer(this->modelsMeta, modelsMetaCapacityBytes, this->memory, header);

	}

	void Data::reset() noexcept
	{
		Dod::BufferUtils::flush(this->xCoords);
		Dod::BufferUtils::flush(this->yCoords);
		Dod::BufferUtils::flush(this->modelsMeta);
	}

	void Data::merge(const Data& other) noexcept
	{
		Dod::BufferUtils::append(this->xCoords, Dod::BufferUtils::createImFromBuffer(other.xCoords));
		Dod::BufferUtils::append(this->yCoords, Dod::BufferUtils::createImFromBuffer(other.yCoords));
		Dod::BufferUtils::append(this->modelsMeta, Dod::BufferUtils::createImFromBuffer(other.modelsMeta));
	}

};
