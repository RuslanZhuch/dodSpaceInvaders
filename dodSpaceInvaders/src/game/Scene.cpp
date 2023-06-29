#include "Scene.h"

#include <engine/contextUtils.h>

namespace Game::Context::Scene
{

	void Data::load() noexcept
	{

        const auto doc{ Engine::ContextUtils::loadFileDataRoot("resources/contexts/sceneContext.json") };
        const auto& inputDataOpt{ Engine::ContextUtils::gatherContextData(doc, 5) };

        if (!inputDataOpt.has_value())
            return;

        const auto& loadingDataArray{ inputDataOpt.value() };

        Engine::ContextUtils::loadVariable(this->topPlaneY, loadingDataArray, 0);
        Engine::ContextUtils::loadVariable(this->topPlaneDir, loadingDataArray, 1);
        Engine::ContextUtils::loadVariable(this->padding, loadingDataArray, 2);
        Engine::ContextUtils::loadVariable(this->bottomPlaneY, loadingDataArray, 3);
        Engine::ContextUtils::loadVariable(this->bottomPlaneDir, loadingDataArray, 4);

	}

	void Data::reset() noexcept
	{

	}

	void Data::merge(const Data& other) noexcept
	{

	}

};
