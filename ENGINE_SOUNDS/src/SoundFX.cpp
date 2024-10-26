#include "Sounds/Essentials/SoundFX.hpp"


ENGINE_SOUNDS::SoundFX::SoundFX(const SoundParams& params, SoundFxPtr pSoundFx)
    : m_Params{params}, m_pSoundFx{ std::move(pSoundFx)}
{

}