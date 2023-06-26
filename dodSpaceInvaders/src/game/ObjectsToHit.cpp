#include "ObjectsToHit.h"

#include <engine/contextUtils.h>

namespace Game::Context::ObjectsToHit
{

	void Data::load() noexcept
	{

		const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/objectsToHit.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 1) };

		if (!inputDataOpt.has_value())
			return;

		const auto& loadingDataArray{ inputDataOpt.value() };

		const auto objectsToHitCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 0) };

		int32_t needBytes{};
		needBytes += objectsToHitCapacityBytes;

		this->memory.allocate(needBytes);
		int32_t header{};

		Engine::ContextUtils::loadBuffer(this->objectsToHit, objectsToHitCapacityBytes, this->memory, header);

	}

	void Data::reset() noexcept
	{
		Dod::BufferUtils::flush(this->objectsToHit);
	}

	void Data::merge(const Data& other) noexcept
	{
		Dod::BufferUtils::append(this->objectsToHit, Dod::BufferUtils::createImFromBuffer(other.objectsToHit));
	}

}
