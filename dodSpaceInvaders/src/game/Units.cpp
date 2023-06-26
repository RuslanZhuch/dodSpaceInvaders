#include "Units.h"

namespace Game::Context::Units
{

	void Data::load()
	{

		const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/soundsController.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 4) };

		if (!inputDataOpt.has_value())
			return;

		const auto& loadingDataArray{ inputDataOpt.value() };

		const auto soundIdsToPlayCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 0) };

		int32_t needBytes{};
		needBytes += soundIdsToPlayCapacityBytes;

		this->memory.allocate(needBytes);
		int32_t header{};

		Engine::ContextUtils::loadBuffer(this->soundIdsToPlay, needBytes, this->memory, header);




		this->memory.allocate(1024 * 10);
		int32_t header{};
		constexpr int32_t totalElements{ 512 };
		Dod::BufferUtils::initFromMemory(this->xCoords, Dod::MemUtils::stackAquire(this->memory, totalElements * sizeof(float), header));
		Dod::BufferUtils::initFromMemory(this->yCoords, Dod::MemUtils::stackAquire(this->memory, totalElements * sizeof(float), header));
	}

	void Data::reset()
	{
		Dod::BufferUtils::flush(this->xCoords);
		Dod::BufferUtils::flush(this->yCoords);
	}

	void Data::merge(const Data& other)
	{
		Dod::BufferUtils::append(this->xCoords, Dod::BufferUtils::createImFromBuffer(other.xCoords));
		Dod::BufferUtils::append(this->yCoords, Dod::BufferUtils::createImFromBuffer(other.yCoords));
		this->width = other.width;
		this->height = other.height;
	}

}
