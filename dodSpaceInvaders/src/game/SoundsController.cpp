#include "SoundsController.h"

namespace Game::Context::SoundsController
{

	void Data::load()
	{

		const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/soundsController.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 1) };

		if (!inputDataOpt.has_value())
			return;

		const auto& loadingDataArray{ inputDataOpt.value() };

		const auto soundIdsToPlayCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 0) };

		int32_t needBytes{};
		needBytes += soundIdsToPlayCapacityBytes;

		this->memory.allocate(needBytes);
		int32_t header{};

		Engine::ContextUtils::loadBuffer(this->soundIdsToPlay, needBytes, this->memory, header);
		
	}

	void Data::reset()
	{
		Dod::BufferUtils::flush(this->soundIdsToPlay);
	}

	void Data::merge(const Data& other)
	{
		Dod::BufferUtils::append(this->soundIdsToPlay, Dod::BufferUtils::createImFromBuffer(other.soundIdsToPlay));
	}

}
