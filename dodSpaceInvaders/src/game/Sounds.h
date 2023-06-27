#pragma once

#include <dod/Buffers.h>
#include <dod/MemPool.h>

#include <Soloud/include/soloud.h>
#include <Soloud/include/soloud_wav.h>

namespace Game::Context::Sounds
{

	struct Data
	{
		void load() noexcept;
		void reset() noexcept;
		void merge(const Data& other) noexcept;

		Dod::MemPool memory;

		std::unique_ptr<SoLoud::Soloud> soundsCore{};
		Dod::DBBuffer<SoLoud::Wav> sounds;
	};

};
