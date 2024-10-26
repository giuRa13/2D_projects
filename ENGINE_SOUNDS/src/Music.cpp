#include "Sounds/Essentials/Music.hpp"


ENGINE_SOUNDS::Music::Music(const SoundParams& params, MusicPtr pMusic)
    : m_Params{params}, m_pMusic{ std::move(pMusic) }
{

}