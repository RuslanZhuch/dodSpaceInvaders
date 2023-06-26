#include "ModelsToRender.h"

#include <engine/contextUtils.h>

namespace Game::Context::ModelsToRender
{
	void Data::load() noexcept
	{

		const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/modelsToRender.json") };
		const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 3) };

		if (!inputDataOpt.has_value())
			return;

		const auto& loadingDataArray{ inputDataOpt.value() };

		const auto loadedModelsCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<sf::VertexArray>(loadingDataArray, 0) };
		const auto modelNamesCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<std::array<char, 64>>(loadingDataArray, 1) };
		const auto modelIdsCapacityBytes{ Engine::ContextUtils::getBufferCapacityBytes<int32_t>(loadingDataArray, 2) };

		int32_t needBytes{};
		needBytes += loadedModelsCapacityBytes;
		needBytes += modelNamesCapacityBytes;
		needBytes += modelIdsCapacityBytes;

		this->memory.allocate(needBytes);
		int32_t header{};

		Engine::ContextUtils::loadBuffer(this->loadedModels, loadedModelsCapacityBytes, this->memory, header);
		Engine::ContextUtils::loadBuffer(this->modelNames, modelNamesCapacityBytes, this->memory, header);
		Engine::ContextUtils::loadBuffer(this->modelIds, modelIdsCapacityBytes, this->memory, header);

	}

	void Data::reset() noexcept
	{
		Dod::BufferUtils::flush(this->loadedModels);
		Dod::BufferUtils::flush(this->modelNames);
		Dod::BufferUtils::flush(this->modelIds);
	}

	void Data::merge(const Data& other) noexcept
	{
		Dod::BufferUtils::append(this->loadedModels, Dod::BufferUtils::createImFromBuffer(other.loadedModels));
		Dod::BufferUtils::append(this->modelNames, Dod::BufferUtils::createImFromBuffer(other.modelNames));
		Dod::BufferUtils::append(this->modelIds, Dod::BufferUtils::createImFromBuffer(other.modelIds));
	}
}
