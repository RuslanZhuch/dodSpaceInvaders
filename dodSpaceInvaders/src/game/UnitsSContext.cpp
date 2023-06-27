#include "UnitsSContext.h"

#include <engine/contextUtils.h>

namespace Game::Context::Units
{

	void Data::load() noexcept
	{

		const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/units.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 5) };

		if (!inputDataOpt.has_value())
			return;

		const auto& loadingDataArray{ inputDataOpt.value() };

		const auto xCoordsCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<float>(loadingDataArray, 0) };
		const auto yCoordsCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<float>(loadingDataArray, 1) };
		const auto groupWidthCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<float>(loadingDataArray, 2) };
		const auto groupHeightCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<float>(loadingDataArray, 3) };
		const auto elementsInGroupCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 4) };

		int32_t needBytes{};
		needBytes += xCoordsCapacityBytes;
		needBytes += yCoordsCapacityBytes;
		needBytes += groupWidthCapacityBytes;
		needBytes += groupHeightCapacityBytes;
		needBytes += elementsInGroupCapacityBytes;

		this->memory.allocate(needBytes);
		int32_t header{};

		Engine::ContextUtils::loadBuffer(this->xCoords, xCoordsCapacityBytes, this->memory, header);
		Engine::ContextUtils::loadBuffer(this->yCoords, yCoordsCapacityBytes, this->memory, header);
		Engine::ContextUtils::loadBuffer(this->groupWidth, groupWidthCapacityBytes, this->memory, header);
		Engine::ContextUtils::loadBuffer(this->groupHeight, groupHeightCapacityBytes, this->memory, header);
		Engine::ContextUtils::loadBuffer(this->elementsInGroup, elementsInGroupCapacityBytes, this->memory, header);

	}

	void Data::reset() noexcept
	{
		Dod::BufferUtils::flush(this->xCoords);
		Dod::BufferUtils::flush(this->yCoords);
		Dod::BufferUtils::flush(this->groupWidth);
		Dod::BufferUtils::flush(this->groupHeight);
		Dod::BufferUtils::flush(this->elementsInGroup);
	}

	void Data::merge(const Data& other) noexcept
	{
		Dod::BufferUtils::append(this->xCoords, Dod::BufferUtils::createImFromBuffer(other.xCoords));
		Dod::BufferUtils::append(this->yCoords, Dod::BufferUtils::createImFromBuffer(other.yCoords));
		Dod::BufferUtils::append(this->groupWidth, Dod::BufferUtils::createImFromBuffer(other.groupWidth));
		Dod::BufferUtils::append(this->groupHeight, Dod::BufferUtils::createImFromBuffer(other.groupHeight));
		Dod::BufferUtils::append(this->elementsInGroup, Dod::BufferUtils::createImFromBuffer(other.elementsInGroup));
	}

}
