#pragma once

#include "Sounds/Essentials/SoundParams.hpp"
#include <EngineUtils/SDL_Wrappers.hpp>


namespace ENGINE_SOUNDS
{

    class SoundFX
    {

    private:
        SoundParams m_Params;
        SoundFxPtr m_pSoundFx;


    public:
        SoundFX(const SoundParams& params, SoundFxPtr pSoundFx);
        ~SoundFX() = default;

        inline const std::string& GetName() const { return m_Params.name; }
        inline const std::string& GetDescription() const { return m_Params.description; }
        inline const std::string& GetFilename() const { return m_Params.filename; }
        inline const double GetDuration() const { return m_Params.duration; }
        inline Mix_Chunk* GetSoundFxPtr() const { if(!m_pSoundFx) return nullptr; return m_pSoundFx.get(); }
    };

}