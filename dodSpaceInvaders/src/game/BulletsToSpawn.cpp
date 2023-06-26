#include "BulletsToSpawn.h"

#include <engine/contextUtils.h>

namespace Game::Context::BulletsToSpawn
{

	void Data::load() noexcept
	{

		const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/bulletsToSpawn.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 2) };

		if (!inputDataOpt.has_value())
			return;

		const auto& loadingDataArray{ inputDataOpt.value() };

		const auto xCoordsCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 0) };
		const auto yCoordsCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 1) };

		int32_t needBytes{};
		needBytes += xCoordsCapacityBytes;
		needBytes += yCoordsCapacityBytes;

		this->memory.allocate(needBytes);
		int32_t header{};

		Engine::ContextUtils::loadBuffer(this->xCoords, xCoordsCapacityBytes, this->memory, header);
		Engine::ContextUtils::loadBuffer(this->yCoords, yCoordsCapacityBytes, this->memory, header);

	}

	void Data::reset() noexcept
	{
		Dod::BufferUtils::flush(this->xCoords);
		Dod::BufferUtils::flush(this->yCoords);
	}

	void Data::merge(const Data& other) noexcept
	{
		Dod::BufferUtils::append(this->xCoords, Dod::BufferUtils::createImFromBuffer(other.xCoords));
		Dod::BufferUtils::append(this->yCoords, Dod::BufferUtils::createImFromBuffer(other.yCoords));
	}

}
