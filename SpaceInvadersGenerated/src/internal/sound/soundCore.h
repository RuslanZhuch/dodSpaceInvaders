#pragma once

#include <Soloud/include/soloud.h>
#include <Soloud/include/soloud_wav.h>

#include <memory>

#undef max
#undef min
#undef GetObject

namespace Game::Sound
{

	using Core = std::unique_ptr<SoLoud::Soloud>;
	using Wav = SoLoud::Wav;

}
